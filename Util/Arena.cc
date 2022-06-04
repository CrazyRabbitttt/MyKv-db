#include "Util/Arena.h"

namespace kvdb {

static const int kBlockSize = 4096;

Arena::Arena() 
    :alloc_ptr_(nullptr), alloc_bytes_remaining_(0), memory_usage_(0) {}

Arena::~Arena() {
    for (size_t i = 0; i < blocks_.size(); i++) {
        delete[] blocks_[i];
    }
}

//进行新的内存的设置
char* Arena::AllocateFallback(size_t bytes) {
    //如果说需要的内存是大于1K，重新分配,但是不会浪费掉原来的剩余的空间
    if (bytes > kBlockSize / 4) {
        char* result = AllocateNewBlock(bytes);
        return result;
    }

    //如果不是的话，直接浪费掉(浪费的空间也是小于1K的，因为已知剩余的空间是小于bytes的)
    alloc_ptr_ = AllocateNewBlock(kBlockSize);
    alloc_bytes_remaining_ = kBlockSize;

    //从新设置的内存进行分配
    char* result = alloc_ptr_;
    alloc_ptr_ += bytes;
    alloc_bytes_remaining_ -= bytes;
    return result;
}

//进行内存对齐的分配
char* Arena::AllocateAligned(size_t bytes) {
    const int align = (sizeof(void*) > 8 ? sizeof(void*) : 8);
    //位运算技巧，判断是否是偶数
    static_assert((align & (align - 1)) == 0, "Pointer size should be a power of 1");
    //位运算技巧：进行取模运算
    size_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr_) & (align - 1);
    size_t slop = (current_mod == 0 ? 0 : align - current_mod);

    //下面进行内存的分配
    size_t needed = bytes + slop;
    char* result;
    if(needed <= alloc_bytes_remaining_) {
        result = alloc_ptr_ + slop;
        alloc_ptr_ += needed;
        alloc_bytes_remaining_ -= needed;
    } else {
        //Fall Back 肯定是返回的是对齐了的
        result = AllocateFallback(bytes);
    }

    assert((reinterpret_cast<uintptr_t>(result) & (align - 1)) == 0);
    return result;
}


//进行新的内存的分配
char* Arena::AllocateNewBlock(size_t block_bytes) {
    char* result = new char[block_bytes];
    blocks_.push_back(result);
    memory_usage_.fetch_add(block_bytes + sizeof(char*), std::memory_order_relaxed);
    return result;
}

}   //namespace kvdb