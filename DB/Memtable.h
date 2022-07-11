#ifndef KVDB_MEMTABLE_H_
#define KVDB_MEMTABLE_H_

#include <string>
#include "../DB/Dbformat.h"
#include "../DB/SkipList.h"
#include "../Util/Arena.h"
#include "../Include/Status.h"


namespace kvdb {

class InternalKeyComparator;

class Memtable{
 public:

 //MemTable是通过引用计数来保证不会被异常消除掉的。初始化为Zero， 调用者至少调用一次Ref()
 //引用计数是0， 就消除掉
 explicit Memtable(const InternalKeyComparator& comparator);


 //No Copying 
 Memtable(const Memtable&) = delete;
 Memtable& operator=(const Memtable&) = delete;


 //Increase reference count.
 void Ref() {
    ++ref_;   
 }

 void UnRef() {
    //judge whether the ref is equal to 0
    --ref_;
    assert(ref_ >= 0);
    if (ref_ <= 0) {
        delete this;
    }
 }

//将seq, type, key, value压缩过后写到buf中，然后插入到跳表上面
void Add(SequenceNumber seq, ValueType type, const Slice& key, const Slice& value);

bool Get(const LookupKey& key, std::string* value, Status *status);


 private:
//   friend class MemTableTierator;

  struct KeyComparator{
    //内部的比较器还是InternalKeyCom..........
    const InternalKeyComparator comparator;
    explicit KeyComparator(const InternalKeyComparator& c) 
        :comparator(c) {}

    //?????????
    // int operator()(const char* a, const char* b) const;
  };
 
  typedef SkipList<const char*, KeyComparator> Table;

  int ref_;     //引用计数
   
  ~Memtable();       //私有：因为只有UnRef() 才调用析构函数
  
  KeyComparator comparator_;
  Arena arena_;
  Table table_;
 

};




}

#endif