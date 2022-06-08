#include "../Include/Status.h"
#include <iostream>

using namespace std;
using namespace kvdb;

int main()
{

    
    kvdb::Status status;
    string s = "I'm a Slicefotestinglala";
    kvdb::Slice slice(s);
    slice.Print();



    status = kvdb::Status::Corruption(slice);         //silice & default slcie

    if (status.IsCorruption()) {
        cout << "Type of the slice : Corruption\n";
    } else if (status.IsNotFound()) {
        cout << "Type of the silce : NotFound\n";
    } else {
        cout << "type error\n";
    }


    string Final = status.ToString();
    cout << Final << endl;



    return 0;
}