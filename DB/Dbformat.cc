#include "Dbformat.h"
#include "../Util/Coding.h"

namespace kvdb {

    //将seq, type 进行打包
    static int PackSeqAndType(uint64_t seq, ValueType type) {
        //7-byte seq,    1-byte type 
        assert(seq <= kMaxSequenceNumber);
        assert(type <= 1);                      //0x0 : Delete,    0x1 : Value
        return (seq << 8) | type;
    }    

    const char* InternalKeyComparator::Name() const {
        return "kvdb.InternalKeyComparator";
    }
    
    int InternalKeyComparator::Compare(const Slice& akey, const Slice& bey) const {
        //Order :
        //1. user key(大的优先)
        //2. seq number(小的优先)
        //3. type

        //还是调用传入的比较的类的方法
        int result = user_comparator_->Compare(ExtractUserKey(akey), ExtractUserKey(bey));
        if (result == 0) {
            const uint64_t anum = DecodeFixed64(akey.data() + akey.size() - 8);
            const uint64_t bnum = DecodeFixed64(bey.data()  + bey.size()  - 8);
            if (anum > bnum) {
                result = -1;
            } else if (anum < bnum) {
                result = +1;
            }
        }
        return result;
    }


    void InternalKeyComparator::FindShortestSeparator(std::string* start, const Slice& limit) const {
        //用于缩减user 的key 部分
        printf("todo: internal key Find1...\n");
    }

    void InternalKeyComparator::FindShortSuccessor(std::string* key) const {
        printf("todo: internalKey Find...\n");
    }
    
    LookupKey::LookupKey(const Slice& user_key, SequenceNumber sequence) {
        size_t usize  = user_key.size();
        size_t needed = usize + 15;     //保守的估计，实际上一般用不了这么多

        char* dst;
        if (needed <= sizeof(space_)) {
            dst = space_;
        } else {
            dst = new char[needed];
        }

        //1. keysize_
        size_ = dst;            //size_是开始的位置
        dst = EncodeVarint32(dst, usize + 8);       //usersize + 8byte

        //2. userkey
        userKey_ = dst;         //下面接上size_之后的位置
        std::memcpy(dst, user_key.data(), usize);   //将userkey写过来

        //3. 8-byte (seq + type)
        dst += usize;                      
        EncodeFixed64(dst, PackSeqAndType(sequence, kValueTypeForSeek));                       //将seq & type(valueType)一起写到dst中

        dst += 8;
        end_ = dst;                                 //the end of the dst 
    }


}