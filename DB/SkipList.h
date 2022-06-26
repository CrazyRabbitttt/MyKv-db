
#ifndef STORAGE_KVDB_SKIPLIST_H_
#define STORAGE_KVDB_SKIPLIST_H_


#include <atomic>
#include <cassert>
#include <cstdlib>

#include "../Util/Arena.h"
#include "../Util/Random.h"


namespace kvdb {

template<typename Key, class Comparatpr>
class SkipList {
private:
    struct Node;            //内部的node类

public:
    //跳表需要有一个用于比较的类， 和内存的分配器
    explicit SkipList(Comparatpr cmp, Arena* arena);

    //Refuse copy behavior 
    SkipList(const SkipList&) = delete;
    SkipList& operator =(const SkipList&) = delete;

    //need: 没有相同的部分
    void Insert(const Key& key);

    //Return true iff equls to key in the list 
    bool Contains(const Key& key) const;

private:

    Node* FindgreaterOrEqual(const Key& key, Node** prev) const;

    static const int kMaxHeight = 12;


    Comparatpr const compare_;

    Arena* const arean_;        //用于进行node的内存的分配

    Node* const header_;

    int RandomHeight();

    Random rand_;           //random 

};


template<typename Key, class Comparator> 
bool SkipList<Key, Comparator>::Contains(const Key& key) const {
    Node* x = Find
}


template<typename Key, class Comparator> 
struct SkipList<Key, Comparator>::Node {
    explicit Node(const Key& k) :key(k) {}

    Key const key;              //Node节点的数值


///////////////////用合适的Method进行封装 -> 从而调用合适的barrier//////////////
    Node* Next(int n) {
        assert(n >= 0);
        return next_[n].load(std::memory_order_acquire);
    }

    //Anyone read can observe the full version 
    void SetNext(int n, Node* x) {
        assert(n >= 0);
        next_[n].store(x, std::memory_order_release);
    }

    //no barrier: read next node 
    Node* NoBarrier_Next(int n) {
        assert(n >= 0);
        return next_[n].load(std::memory_order_relaxed);
    }

    //no barrier: store the node
    void NoBarrier_SetNext(int n, Node* x) {
        assert(n >= 0);
        next_[n].store(x, std::memory_order_relaxed);
    }


private:
    
    //next[0] 就是最低层次的指针, 数组的值就是高度？
    std::atomic<Node*> next_[1];

};





//获得随机的高度，随机进行高度的增长，最高12层
template<typename Key, class Comparator>
int SkipList<Key, Comparator>::RandomHeight() {
    //1/4的概率进行高度的增长
    static const unsigned int kBranching = 4;

    int height = 1;
    //height的增长概率是四分之一
    while (height < kMaxHeight && ((rand_.Next() % kBranching) == 0)) {
        height++;
    }
    assert(height > 0);
    assert(height <= kMaxHeight);
    return height;
}

template<typename Key, class Comparator> 
typename SkipList<Key, Comparator>::Node* 








// template<typename Key, class Comparator>
// bool SkipList<Key, Comparator>::Contains(const Key& key) const {
    
// }

















}

#endif