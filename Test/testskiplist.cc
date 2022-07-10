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
    
    // kvdb::Memtable memtable1;
    // kvdb::compare::ByteComparator com1;
    Comparator1 com2;
    typedef kvdb::SkipList<const Key, Comparator1> Table;
    kvdb::Arena arean_;

    Table table(com2, &arean_);

void testInsertAndContains() {
    printf("Begin test insert and contains ...\n");
    int count = 0;
    int grade = 0;
    for (int i = 50; i <= 150; i++) {
        table.Insert(i);
        count ++;
    }
    assert(count == 101);
    if (count == 101) grade++;
    // printf("Begin judge if contains..., the current count : %d\n", count);
    for (int i = 50; i <= 150; i++) {
        if (table.Contains(i)) count --;
    }
    assert(count == 0);
    if (count == 0) grade ++;
    // printf("The final count %d, should 0\n", count);

    int fins = 0;
    for (int i = 1; i <= 500; i++) {
        if (table.Contains(i) && !(i >= 50 && i <= 150)) {
            fins++;
        }
    }
    assert(fins == 0);
    if (fins == 0) grade++;

    printf("There are three tests, pass grade %d / 3\n", grade);
}


void testIterator() {
    printf("Begin running test iterator...\n");
  SkipList<const Key, Comparator1>::Iterator iter(&table);
  if (iter.Valid()) printf("error, init iter should not valid\n");
  int grade = 0;
  iter.Seek(99);
  assert(iter.Valid());
  const Key& tmpkey1 = iter.key();
  if (tmpkey1 == 99) grade++;
//   printf("The first key which after 99 : %d\n", tmpkey1);

  iter.SeekForFirst();
  const Key& tmpkey2 = iter.key();
//   printf("The first key : %d\n", tmpkey2);
  if (tmpkey2 == 50) grade++;
 
  iter.SeekForLast();
  const Key& tmpkey3 = iter.key();
//   printf("The last key : %d\n", tmpkey3);
  if (tmpkey3 == 150) grade++;
  
  printf("There are three tests, pass grade %d / 3\n", grade);

}




int main()
{
    testInsertAndContains();

    testIterator();
}

