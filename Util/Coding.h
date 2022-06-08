#ifndef KVDB_UTIL_CODING_H_
#define KVDB_UTIL_CODING_H_

/*
进行uint_t类型的编码 & 解码
引入Slice类型
*/


#include <cstdint>
#include <cstring>
#include <string>
#include "../Include/Slice.h"

namespace kvdb {


//低版本的PUT。。。直接将内容写入缓冲区
//要求：dst需要拥有足够的空间

inline void EncodeFixed32(char* dst, u_int32_t value) {
    //类型转换，底层进行解释的不同，解释为：按照8位的类型进行解释
    uint8_t* const buffer = reinterpret_cast<uint8_t*>(dst);

        /*
            每次都会将最后面的一个字节清除掉，小端，存储到buf中
                4  2  5  6
        buf[0]:          |

        buf[1]:       |

        buf[2]:    |

        buf[3]: |

        */
    buffer[0] = static_cast<uint8_t>(value);
    buffer[1] = static_cast<uint8_t>(value >> 8);
    buffer[2] = static_cast<uint8_t>(value >> 16);
    buffer[3] = static_cast<uint8_t>(value >> 24);
}

inline void EncodeFixed64(char* dst, uint64_t value) {
  uint8_t* const buffer = reinterpret_cast<uint8_t*>(dst);

  //同上述函数，将内容解析到buf中
  buffer[0] = static_cast<uint8_t>(value);
  buffer[1] = static_cast<uint8_t>(value >> 8);
  buffer[2] = static_cast<uint8_t>(value >> 16);
  buffer[3] = static_cast<uint8_t>(value >> 24);
  buffer[4] = static_cast<uint8_t>(value >> 32);
  buffer[5] = static_cast<uint8_t>(value >> 40);
  buffer[6] = static_cast<uint8_t>(value >> 48);
  buffer[7] = static_cast<uint8_t>(value >> 56);
}


inline u_int32_t DecodeFixed32(const char* ptr) {
    const uint8_t* const buffer = reinterpret_cast<const uint8_t*>(ptr);

    return  static_cast<uint32_t>(buffer[0]) |  
            static_cast<uint32_t>(buffer[1] << 8)  |
            static_cast<uint32_t>(buffer[2] << 16) |
            static_cast<uint32_t>(buffer[3] << 24);
}

inline u_int64_t DecodeFixed64(const char* ptr) {
    const uint8_t* const buffer = reinterpret_cast<const uint8_t*>(ptr);

  // Recent clang and gcc optimize this to a single mov / ldr instruction.
  return (static_cast<uint64_t>(buffer[0])) |
         (static_cast<uint64_t>(buffer[1]) << 8) |
         (static_cast<uint64_t>(buffer[2]) << 16) |
         (static_cast<uint64_t>(buffer[3]) << 24) |
         (static_cast<uint64_t>(buffer[4]) << 32) |
         (static_cast<uint64_t>(buffer[5]) << 40) |
         (static_cast<uint64_t>(buffer[6]) << 48) |
         (static_cast<uint64_t>(buffer[7]) << 56);
}


}

#endif