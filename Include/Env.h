#ifndef _KVDB_INCLUDE_ENV_H_
#define _KVDB_INCLUDE_ENV_H_

#include <stdarg.h>
#include <cstdint>
#include <string>
#include <vector>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <queue>
#include <set>
#include <string.h>
#include <thread>
#include <type_traits>
#include <utility>

#include "../Include/Status.h"


namespace kvdb {

//WriteFile buf size : 64k
constexpr const size_t kWritableFileBufferSize = 65536;
//define the abstract of WriteTableFile

class WritableFile {
public:
    WritableFile() = default;

    //禁止拷贝的行为
    WritableFile(const WritableFile&) = delete;
    WritableFile& operator=(const WritableFile&) = delete;

    virtual ~WritableFile();        //虚析构函数

    //纯虚函数后面加上0，子类必须进行重写
    virtual Status Append(const Slice& data) = 0;
    virtual Status Close() = 0;
    virtual Status Flush() = 0;
    virtual Status Sync()  = 0;

};




}


#endif