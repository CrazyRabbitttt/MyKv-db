#include "Memtable.h"
#include "Dbformat.h"
#include "../Include/Env1.h"
#include "../Util/Coding.h"
#include "../Util/Coding.cc"
#include "../Include/Comparator.h"


namespace kvdb {

 void Memtable::Add(SequenceNumber seq, ValueType type, const Slice& userkey, const Slice& value) {

    /*
      KeySize: 32bit 
      KeyByte: char[keysize]
      Tag    : uint64((seq << 8) | type) 
      v-size : 32bit 
      v-Byte : char[valuesize]
    */
    size_t key_size   = userkey.size();
    size_t value_size = value.size();
    size_t internal_key_size = key_size + 8;      //user key size + 8 byte(seq, valueType)
    
    //值进行很好的压缩，最终需要的存储的空间大小是多少(Byte
    const size_t encoded_len = VarintLength(internal_key_size) + internal_key_size + 
                               VarintLength(value_size) + value_size;

    char* buf = arena_.Allocate(encoded_len);

    //1. internal size
    char* p   = EncodeVarint32(buf, internal_key_size);

    //2. Userkey value
    std::memcpy(p, userkey.data(), key_size);

    p += key_size;
    //3. Seqnumber & type [固定8字节，所以是64-bit]
    EncodeFixed64(p, (seq << 8) | type);

    p += 8;   //fixed 8 bytes

    //4. Value size
    p = EncodeVarint32(p, value_size);
   
    //5. Value 
    std::memcpy(p, value.data(), value_size);

    //进行断言检测
    assert(p + value_size == buf + encoded_len);
    
    table_.Insert(buf);         //调用SkipList的插入

 } 



}