#include "../Include/Comparator.h"
#include "../Include/Slice.h"
#include <algorithm>
#include <string>
#include <cstdint>
#include <type_traits>
#include <stdio.h>

namespace kvdb {

// Comparator::~Comparator() = default;


//匿名命名空间， 相当于是internal 
namespace  {
    //用字节顺序进行比较的比较器
class ByteComparator : public Comparator {
 public:
    ByteComparator() = default;

    ~ByteComparator() {
        printf("byteComparaotr 析构函数...\n");
    }

    const char* Name() const override { return "kvdb.ByteComparator";}

    //call the function in Slcie
    int Compare(const Slice& a, const Slice& b) const override {
        a.compare(b);
    }

    //find small between [start, limit)
    //将*start换成是一个很小的[start, limit)的字符串，字节顺序比较
    void FindShortestSeparator(std::string* start, const Slice& limit) const override {
        //Find the common prefix, 公共前缀
        size_t min_len = std::min(start->size(), limit.size());
        size_t index = 0;
        while (index != min_len && ((*start)[index] == limit[index])) {
            index++;
        }

        if (index >= min_len) {
            //Do nothing, 一个字符串是另一个的子集（缩减不了了）
        } else {
            //不是的话，将除了前缀之后的进行uint8 + 1
            uint8_t diff_byte = static_cast<uint8_t>((*start)[index]);

            if (diff_byte < static_cast<uint8_t>(0xff) && diff_byte + 1 < static_cast<uint8_t>(limit[index])) { 
                (*start)[index] ++;     //前缀之后的字节数目加上1
                start->resize(index+1);
                assert(Compare(*start, limit) < 0);
            }
        }
    }

    void FindShortSuccessor(std::string* key) const override {
        //Find first 
        size_t n = key->size();
        for (int i = 0; i < n; i++) {
            const uint8_t byte = (*key)[i];
            if (byte != static_cast<uint8_t>(0xff)) {
                (*key)[i] = byte + 1;
                key->resize(i + 1);
                return;
            }
        }
    }
};

}



}
