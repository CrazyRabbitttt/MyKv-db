#ifndef _KVDB_UTIL_RANDOM_H_
#define _KVDB_UTIL_RANDOM_H_
#include <cstdint>
namespace kvdb {

class Random {
private:
    uint32_t seed_;

public:
    explicit Random(uint32_t s) :seed_(s & 0x7fffffffu) {
        //避免Bad Seed
        if (seed_ == 0 || seed_ == 2147483647L) {
            seed_ = 1;
        }
    }  

    uint32_t Next() {
        static const uint32_t M = 2147483647L;      //2^31-1
        static const uint64_t A = 16807;
    }


};


}



#endif