#include "Dbformat.h"
#include "../Util/Coding.h"

namespace kvdb {

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