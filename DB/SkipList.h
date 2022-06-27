
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
    static const int kMaxHeight = 12;



//Functions
    //不保证并发性质的读取height
    inline int GetMaxGeight() const {
        return max_align_t.load(std::memory_order_relaxed);
    }

    bool Equal(const Key& a, const Key& b) const {
        return (compare_(a, b) == 0);
    }


    //Return true if key > node->value 
    bool KeyIsAfterNode(const Key& key, Node* node) const;

    Node* FindgreaterOrEqual(const Key& key, Node** prev) const;

    Node* NewNode(const Key& key, int height);


//Varaiable 
    Comparatpr const compare_;

    Arena* const arena_;        //用于进行node的内存的分配

    Node* const header_;

    int RandomHeight();

    std::atomic<int> max_height_;

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



template<typename Key, class Comparator> 
typename SkipList<Key, Comparator>::Node* SkipList<Key, Comparator>::NewNode(const Key& key, int height) {
    //因为最底层Node节点已经有了node* ，因此还是需要height -1 个
    char* const node_mem = arena_->AllocateAligned(sizeof(Node) + sizeof(std::atomic<Node*>) * (height - 1));
    return new(node_mem) Node(key);
} 



template<typename Key, class Comparator> 
SkipList<Key, Comparator>::SkipList(Comparator cmp, Arena* arena) 
    : compare_(cmp), arena_(arena), header_(NewNode(0, kMaxHeight)), max_height_(1), rand_(0xdeadbeef) {
        //初始化所有的next
        for (int i = 0; i < kMaxHeight; i++) {
            header_->SetNext(i, nullptr);
        }
    }



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




//每层查找最后一个小于Key的节点
template<typename Key, class Comparator> 
typename SkipList<Key, Comparator>::Node*               //typename 用在这里来指明后面接的是一种类型
SkipList<Key, Comparator>::FindgreaterOrEqual(const Key& key, Node** prev) const {
    //双重指针就相当于单指针的数组
    Node* pNode = header_;               //begin from header node 
    int level = GetMaxGeight() - 1;    //从最高层次开始
    for (int i = level; i >= 0; i--) {
        while (KeyIsAfterNode(key, pNode->Next(i))) {    //在目前层次的下一个
            pNode = pNode -> next_[i];
        }
    }   
    return nullptr;
}

//判断Key的值是不是大于node的值
template<typename Key, class Comparator> 
bool SkipList<Key, Comparator>::KeyIsAfterNode(const Key& key, Node* node) const {
    return (n != nullptr && compare_(n->key, key) < 0);
}





template<typename Key, class Comparator>
bool SkipList<Key, Comparator>::Contains(const Key& key) const {
    Node* tmp = FindgreaterOrEqual(key, nullptr);
    if (x != nullptr && Equal(key, tmp->key)) {
        return true;
    } else {
        return false;
    }
}

}

#endif