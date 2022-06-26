

#include "log_writer.h"
#include <cstdint>
#include <iostream>
#include "../Util/Crc32c.h"
#include "../Util/Coding.h"

namespace kvdb {
namespace log {


    //进行crc校验和的设置, 将校验和放在前四个字节
    static void InitTypeCrc(uint32_t* type_crc) {
        for (int i = 0; i <= kMaxRecordType; i++) {
            char t = static_cast<char>(i);
            type_crc[i] =  Crc32c::Value(&t, 1);
        }
    }


    Writer::Writer(PosixWritableFile* dest) 
        :dest_(dest), block_offset_(0) {
            InitTypeCrc(type_crc_);
        }
    
    Writer::Writer(PosixWritableFile *dest, uint64_t dest_length) 
        :dest_(dest), block_offset_(dest_length % kBlockSize) 
        {InitTypeCrc(type_crc_);}

    Writer::~Writer() = default;



    //具体的将Slice传到Log中
    Status Writer::AddRecord(const Slice& slice) {
        printf("Now running the Add Record function, the silce:%s\n", slice.data());
        const char* ptr = slice.data();
        size_t left = slice.size();

        //Fragment to the Record, 将内容进行分段写入到Log文件中去
        Status s;
        bool begin = true;
        do {    
            const int leftover = kBlockSize - block_offset_;
            assert(leftover >= 0);
            
            if (leftover < kHeaderSize) {       //剩余的空间写头部都不够，用0填充
                //切换到下一个块
                if (leftover > 0) {
                    static_assert(kHeaderSize == 7, "");
                    dest_->Append(Slice("\x00\x00\x00\x00\x00\x00", leftover));
                }
                block_offset_ = 0;
            }

            //处理完之后剩余的部分必须是大于等于一个头部的空间的
            assert(kBlockSize - block_offset_ -  kHeaderSize >= 0);
            
            const size_t avail = kBlockSize - block_offset_ - kHeaderSize;
            const size_t fragment_len = (left < avail) ? left : avail;

            RecordType type;
            //判断是否是end， 如果剩下需要写的Slice同剩下的空间一样就是end
            const bool end = (left == fragment_len);
            if (begin && end) {
                type = kFullType;
            } else if (begin) {
                type = kFirstType;
            } else if (end) {
                type = kLastType;
            } else {
                type = kMidType;
            }

            s = EmitPhysicalRecord(type, ptr, fragment_len);
            ptr += fragment_len;
            left -= fragment_len;
            begin = false;
        }while (s.ok() && left > 0);        //no fault and can write 
    }

    //实际上写到物理磁盘的函数
    Status Writer::EmitPhysicalRecord(RecordType t, const char* ptr, size_t len) {
        assert(len <= 0xffff);      //len规格必须在2bytes之内
        assert(block_offset_ + kHeaderSize + len <= kBlockSize);

        //将Header字段进行填充

        char buf[kHeaderSize];
        // buf[0] = buf[1] = buf[2] = buf[3] = '8';            //暂时将buf中的crc用字符8来进行
        buf[4] = static_cast<char>(len & 0xff);
        buf[5] = static_cast<char>(len >> 8);
        buf[6] = static_cast<char>(t);

        //计算crc校验和
        uint32_t crc = Crc32c::Extend(type_crc_[t], ptr, len);
        crc = Crc32c::Mask(crc);
        EncodeFixed32(buf, crc);        //将四字节编码到buf中
        printf("the header : %s..\n", buf);


        //将头部 和 payload写入
        Status s = dest_->Append(Slice(buf, kHeaderSize));
        if (s.ok()) {       //如果头部字段写入成功
            s = dest_->Append(Slice(ptr, len));
            if (s.ok()) {
                s = dest_->Flush();         //刷到文件中
            }
        }
        block_offset_ += kHeaderSize + len;
        return s;
    }


}    
}