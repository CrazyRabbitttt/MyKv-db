#include "../DB/Memtable.h"
#include "../Util/Comparator.cc"
#include <stdio.h>

using namespace std;
using namespace kvdb;

typedef uint64_t Key;




void test1() {


    const Comparator* com;

    const ByteComparator* com1;


    InternalKeyComparator* internal(com1);

    const char* str  = internal->Name();

    // KeyComparator cmp(internal);

    // const Comparator* com;
    
    // const kvdb::ByteComparator comparator;

    // com = &comparator;          //father's pointer point to child's object 
    // const char* comName = com->Name();

    // printf("The name of comparator : %s\n", comName);

    // InternalKeyComparator cmp(com);
    
    // printf("The name of internal: %s\n", cmp.Name());
   
}

int main() {
    test1();
}



