#ifndef _KVDB_DB_LOG_FORMAT_H_
#define _KVDB_DB_LOG_FORMAT_H_

namespace kvdb {
namespace log {

//写入Log时候分段进行写入
enum RecordType {
    kZeroType = 0,      //为预分配文件所设置的zero
    kFullType = 1,  


    //不同的fragments
    kFirstType = 2,
    kMidType   = 3,
    kLastType  = 4
};

static const int kMaxRecordType = kLastType;

static const int kBlockSize = 32 * 1024;
    //checksum(4), length (2), type(1)
static const int kHeaderSize = 4 + 2 + 1;       //头部字段的值

}
}






#endif