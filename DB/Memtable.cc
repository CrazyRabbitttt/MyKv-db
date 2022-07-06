#include "Memtable.h"
#include "Dbformat.h"
#include "../Include/Env1.h"
#include "../Util/Coding.h"
#include "../Include/Comparator.h"


namespace kvdb {

 void Memtable::Add(SequenceNumber s, ValueType type, const Slice& userkey, const Slice& value) {

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
    char* p   = EncodeVarint32(buf, internal_key_size);
                       



    //TODO：



 } 



}