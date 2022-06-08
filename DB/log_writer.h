#ifndef _KVDB_DB_LOG_WRITER_H_
#define _KVDB_DB_LOG_WRITER_H_

#include <cstdint>

#include "./Log_format.h"
#include "../Include/Slice.h"
#include "../Include/Status.h"
#include "../Include/Env.h"
#include "../Include/Env1.h"

namespace kvdb {

class PosixWritableFile;

namespace log {

class Writer {
public:
    //不允许隐式转换
    explicit Writer(PosixWritableFile* dest);

    //禁止拷贝的行为
    Writer(const Writer& ) = delete;
    Writer& operator=(const Writer&) = delete;

    ~Writer();

    //唯一的调用，将Slice写入到Log中,调用EmitPhy..函数
    Status AddRecord1(const Slice& slice);

private:
    //实际上写到Log中的调用的函数
    Status EmitPhysicalRecord(RecordType type, const char* ptr, size_t length);

    PosixWritableFile *dest_;
    int block_offset_;      //当前的块中的offset

    uint32_t type_crc_[kMaxRecordType + 1];
};


}
}


#endif