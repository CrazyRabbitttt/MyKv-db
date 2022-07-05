#include "Memtable.h"
#include "Dbformat.h"
#include "../Include/Env1.h"
#include "../Util/Coding.h"
#include "../Include/Comparator.h"


namespace kvdb {

 void Memtable::Add(SequenceNumber s, ValueType type, const Slice& key, const Slice& value) {

    /*
      KeySize: 32bit 
      KeyByte: char[keysize]
      Tag    : uint64((seq << 8) | type) 
      v-size : 32bit 
      v-Byte : char[valuesize]
    */
    size_t key_size   = key.size();
    size_t value_size = value.size();
    //TODO：



 } 



}