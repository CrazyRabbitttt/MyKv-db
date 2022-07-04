#ifndef MYKVDB_INCLUDE_COMPARATOR_H_
#define MYKVDB_INCLUDE_COMPARATOR_H_

#include <string>

//比较类的基类

namespace kvdb {

class Slice;

class Comparator {

public:
    virtual ~Comparator();

    virtual int Compare(const Slice& a, const Slice& b) const = 0;

    //The name of comparator, 用于判断不同的比较方式， kvdb开头

    virtual const char* Name() const = 0;

    //[start, limit)之间的一个短的串， 降低存储的空间
    virtual void FindShortestSeparator(std::string* start, const Slice& limit) const = 0;


    //没有上端的限制
    virtual void FindShortSuccessor(std::string* key) const = 0;

};

    //返回的是按照字典序的比较器
    const Comparator* BytewiseComparator();
}

#endif  