#include "Include/Status.h"
#include <iostream>

using namespace std;
int main()
{

    
    kvdb::Status status;
    kvdb::Slice slice("我是一个切片！\n");
    status = kvdb::Status::NotFound(slice);         //silice & default slcie

    if (status.IsCorruption()) {
        cout << "Type of the slice : Corruption\n";
    } else if (status.IsNotFound()) {
        cout << "Type of the silce : NotFound\n";
    } else {
        cout << "type error\n";
    }


    string Final = status.ToString();
    cout << Final;



    return 0;
}