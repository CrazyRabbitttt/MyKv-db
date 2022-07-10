// #include "../DB/Memtable.h"
// #include "../DB/Memtable.cc"
// #include "../Include/Comparator.h"
// #include "../Util/Comparator.cc"
#include "../DB/SkipList.h"
#include "../Util/Arena.cc"
#include <stdio.h>

using namespace kvdb;

typedef uint64_t Key;

struct Comparator1 {
    int operator() (const Key& a, const Key& b) const {         //这里不加const 就报错
        if (a < b) return -1;
        else if (a > b) return +1; 
        else return 0;
    }
    int Compare(const Key& a, const Key& b) const {
        if (a < b) return -1;
        else if (a > b) return +1; 
        else return 0;
    }
};
 
int main()
{
    // kvdb::Memtable memtable1;
    // kvdb::compare::ByteComparator com1;
    Comparator1 com2;
    typedef kvdb::SkipList<const Key, Comparator1> Table;
    kvdb::Arena arean_;

    Table table(com2, &arean_);

    printf("Begin insert...\n");
    for (int i = 89; i < 367; i++) {
        table.Insert(i);
    }
    const int sum = 250;
    int count = 0;
    printf("Begin judge if contains...\n");
    for (int i = 101; i <= 250; i++) {
        if (table.Contains(i)) count ++;
    }

    printf("hit : %d, sum : 150\n", count);

}

