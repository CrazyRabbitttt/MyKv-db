#include "Memtable.h"
#include "Dbformat.h"
#include "../Include/Env1.h"
#include "../Util/Coding.h"
#include "../Util/Coding.cc"
#include "../Include/Comparator.h"


namespace kvdb {
    //跳过size，返回value
static Slice GetLengthPrefixedSlice(const char* data) {
  uint32_t len;
  const char* p = data;
  p = GetVarint32Ptr(p, p + 5, &len);  // +5: we assume "p" is not corrupted
  return Slice(p, len);
}


 
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
    
    //值进行很好的压缩，最终需要的存储的空间大小是多少(Byte]
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

 bool Memtable::Get(const LookupKey& key, std::string* value, Status* status) {
    Slice memkey = key.memtable_key();
    Table::Iterator iter(&table_);
    iter.Seek(memkey.data());             //寻找的是key的部分，最终查找的结果存放在iter类中的node_中
    
    if (iter.Valid()) {
        const char* entry = iter.key();   //从跳表中拿到的entry 
        uint32_t key_length = 0;
        //这个函数会拿到internal_size, 并且跳过size的部分
        const char* userkey_ptr = GetVarint32Ptr(entry, entry + 5, &key_length);
        if (comparator_.comparator.user_Comparator()->Compare(key.user_key(), Slice(userkey_ptr, key_length - 8)) == 0) {
            //传入的userKey同跳表中拿到的userKey是一样的
            //sequence & type
            const uint64_t tag = DecodeFixed64(userkey_ptr + key_length);
            switch (static_cast<ValueType>(tag & 0xff)) {
              case kTypeValue: {
                  Slice v = GetLengthPrefixedSlice(userkey_ptr + key_length);
                  value->assign(v.data(), v.size());
                  return true;
              }
              case kTypeDelete: 
                *status = Status::NotFound(Slice());
                return true;
            }
        }
    } 
    return false;
 }

 



}