#ifndef KVDB_DBFORMAT_H_
#define KVDB_DBFORMAT_H_

#include <string>
#include <stdint.h>

#include "../Include/Comparator.h"
namespace kvdb {

class InternalKey;

//The type of value which is inserted on the SkipList, value Or delete 
enum ValueType {kTypeDelete = 0, kTypeValu = 1 };

//Sequence number, 64 bits   7bytes(sequence), 1byte(type)
typedef uint64_t SequenceNumber;

static const SequenceNumber kMaxSequenceNumber = ((0x1ull << 56) - 1);




class InternalKeyComparator : public Comparator {
 private:
     const Comparator* user_comparator_;        //????

 public:

    explicit InternalKeyComparator(const Comparator* c) 
        :user_comparator_(c) {}
    
    virtual const char* Name() const;

    virtual int Compare(const Slice& a, const Slice& b) const;

    virtual void FindShortestSeparator(std::string* start,
                                     const Slice& limit) const;
    virtual void FindShortSuccessor(std::string* key) const;


    int Compare(const InternalKey& a, const InternalKey& b) const;




};







}
#endif