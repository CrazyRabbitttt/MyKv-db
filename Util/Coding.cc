#include "Coding.h"

namespace kvdb {

    //进行可变参数Int32的压缩 -> dst 
    char* EncodeVarint32(char* dst, uint32_t v) {
        //reinterpret_cast:可以将指针看作是整数
        uint8_t* ptr = reinterpret_cast<uint8_t*>(dst);
        static const int B = 128;      //1 000 | 0000
        if (v < (1 << 7)) {
            *(ptr++) = v;               //最高位就是0， 1Byte
        } else if (v < (1 << 14)) {
            *(ptr++) = v | B;           //没有结束，最高位补1
            *(ptr++) = v >> 7;          //将剩下的填充上去
        } else if (v < (1 << 21)) {
             *(ptr++) = v | B;
            *(ptr++) = (v >> 7) | B;
            *(ptr++) = v >> 14;
        } else if (v < (1 << 28)) {
            *(ptr++) = v | B;
            *(ptr++) = (v >> 7) | B;
            *(ptr++) = (v >> 14) | B;
            *(ptr++) = v >> 21;
        } else {
            *(ptr++) = v | B;
            *(ptr++) = (v >> 7) | B;
            *(ptr++) = (v >> 14) | B;
            *(ptr++) = (v >> 21) | B;
            *(ptr++) = v >> 28;
        }
        return reinterpret_cast<char*>(ptr);

    }

    void PutVarint32(std::string* dst, uint32_t v) {
        char buf[5];
        char* ptr = EncodeVarint32(buf, v);
        dst->append(buf, ptr - buf);    //将压缩过的内容写到dst中去
    }

    char* EncodeVarint64(char* dst, uint64_t v) {
        static const int B = 128;
        uint8_t* ptr = reinterpret_cast<uint8_t*>(dst);
        while(v >= 128) {
            *(ptr++) = v | B;
            v >>= 7;            //因为一个字节中，最终有效的是后面7位，首位是Valid位
        }
        *(ptr++) = static_cast<uint8_t>(v);
        return reinterpret_cast<char*>(ptr);
    }

    void PutVarint64(std::string* dst, uint64_t) {
        char buf[10];
        char* ptr = EncodeVarint64(buf, v);
        dst->append(buf, ptr - buf);
    }


    //返回传入的int的len
    int VarintLength(uint64_t value) {
        //1 2 4 8 16 32 64 128
        int len = 1;
        while (value >= 128) {
            v >>= 7;
            len++;
        }
        return len;
    }






}