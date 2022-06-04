#ifndef KVDB_UTIL_ARENA_H_
#define KVDB_UTIL_ARENA_H_
#include <atomic>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace kvdb {

class Arena {
public:
    Arena();

    //拒绝拷贝的行为
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    ~Arena();

    //返回最新分配内存的指针
    char* Allocate(size_t bytes);

    //对齐分配内存的方式
    char* AllocateAligned(size_t bytes);

    //分配的内存
    size_t MemoryUsage() const {
        return memory_usage_.load(std::memory_order_relaxed);
    }

private:

    //进行内存的分配
    char* AllocateFallback(size_t bytes);
    char* AllocateNewBlock(size_t bytes);

    //Allocation state, 分配的内存的状态
    char* alloc_ptr_;
    size_t alloc_bytes_remaining_;

    //分配了的内存的Array
    std::vector<char*> blocks_;

    //Total memory usage of arena
    std::atomic<size_t> memory_usage_;
};


inline char* Arena::Allocate(size_t bytes) {
    //不允许0字节的分配
    assert(bytes > 0);
    if (bytes <= alloc_bytes_remaining_) {          //剩余的数目可供分配
        char* result = alloc_ptr_;
        alloc_ptr_ += bytes;                        //可分配的内存指针后移
        alloc_bytes_remaining_ -= bytes;
        return result;                              //返回的是初始分配的指针的位置
    }
    return AllocateFallback(bytes);                 //不够，那么重新进行分配
} 

}   //namespace of kvdb


#endif