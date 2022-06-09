#include "../DB/log_writer.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
using namespace std;
using namespace kvdb;
using namespace log;
int main() 
{
    string filename = "testLog";
    int testfd;
    testfd = open(filename.c_str(), O_RDWR | O_APPEND);       //读写的方式进行打开文件

    printf("The fd: %d\n", testfd);
    PosixWritableFile writefile(filename, testfd);

    Writer write(&writefile);
  
    std::string s = "Guagua\n";
    Slice slice(s);
    write.AddRecord1(slice);

}