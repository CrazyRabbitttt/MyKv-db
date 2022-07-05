#include "../Util/Comparator.cc"
#include "../Include/Slice.h"
#include <string.h>
#include <iostream>
using namespace kvdb;
int main()
{
    
    kvdb::compare::ByteComparator com1;
    std::string s = "hellow";
    Slice slice1 = "helloz";


    com1.FindShortestSeparator(&s, slice1);

    std::cout << "The string after: " << s << std::endl;
    std::cout << "The slice  after: " << slice1.data() << std::endl;


    std::string s2 = "hello";
    com1.FindShortSuccessor(&s2);
    std::cout << "After find shor: " << s2 << std::endl;

    return 0;

}