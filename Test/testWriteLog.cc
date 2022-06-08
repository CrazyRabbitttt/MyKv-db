#include "../DB/log_writer.h"
#include <iostream>
using namespace std;
using namespace kvdb;
using namespace log;
int main() 
{
    string filename = "testLog";
    int testfd = 4;
    PosixWritableFile writefile(filename, testfd);

    Writer write(&writefile);

    std::string s = "Lalala, I'm the test\n guagua\n";
    Slice slice(s);
    write.AddRecord1(slice);

}