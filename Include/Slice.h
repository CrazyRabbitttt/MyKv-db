#ifndef KVDB_INCLUDE_SLICE_H_
#define KVDB_INCLUDE_SLICE_H_

//Slice,而不使用String，因为String有默认的拷贝行为
//并且不支持前缀的操作

#include <cassert>
#include <cstddef>
#include <string>
#include <cstring>
// #include "Export.h"

namespace kvdb {


/*
   Slice 只是关心切片的位置 & 位置，不关心切片的实际的内容
   将Slice看作是字节的切片
*/

class  Slice {
 public:
    //创建Empty slice
    Slice():data_(""), size_(0) {}

    //传递参数进行构造 d[0,n-1]
    Slice(const char *d, size_t n) :data_(d), size_(n) {}

    //传递String进行构造
    Slice(const std::string& s) : data_(s.data()), size_(s.size()) {}

    //默认的进行拷贝行为？
    Slice(const Slice&) = default;
    Slice& operator=(const Slice&) = default;

    //返回一些private的参数等
    size_t size() const {return size_;}

    //Return true if length is zero
    bool empty() const {return size_ == 0;}

    //Return the data of Slice
    const char* data() const {return data_;}

    //重载[]
    char operator[](size_t n) const {
       assert(n < size());
       return data_[n];
    }

    //Clear the Slice 
    void Clear() {
       data_ = "";
       size_  = 0;
    }

    //去除掉前n个bytes
    void remove_prefix(size_t n) {
       assert(n <= size());
       data_ += n;
       size_ -= n;
    }

    //Return string type value 
    std::string ToString() const {return std::string(data_, size_);}


    //Comparation the Silce's 
    int compare(const Slice& b) const;


   //判断x是否是*this的前缀
   bool starts_with(const Slice& x) const {
      return((size_ >= x.size_) && (memcmp(data_, x.data_, x.size_) == 0));
   }
   void Print() {
      printf("%s\n", data_);
   }

 private:
    const char* data_;
    size_t size_;
};

inline bool operator==(const Slice& x, const Slice& y) {
   return ((x.size() == y.size()) && (memcmp(x.data(), y.data(), x.size()) == 0));
}

inline bool operator!=(const Slice& x, const Slice& y) {
   return !(x == y);    //调用上面声明了的重载的==
}

//compare two Slices
inline int Slice::compare(const Slice& b) const {
   const size_t len = (b.size_ < size_) ? b.size_ : size_;
   int ret = memcmp(data_, b.data_, len);
   //一个属于另一个的前缀，那么判断size的大小就可以了
   if (ret == 0) {
      if (size_ > b.size_) return 1;
      else return -1;
   }
   return ret;
}

}   //end of the namespace kvdb

#endif