#ifndef _KVDB_INCLUDE_ENV1_H_
#define _KVDB_INCLUDE_ENV1_H_

#include "Env.h"


namespace kvdb {


class PosixWritableFile final : public WritableFile {
 public:
  PosixWritableFile(std::string filename, int fd)
      : pos_(0),
        fd_(fd),
        filename_(std::move(filename))       //将传入的内存进行替换
        {}
  ~PosixWritableFile() override {
    if (fd_ >= 0) {
      // Ignoring any potential errors
      Close();
    }
  }

 private: 
  // buf_[0, pos_ - 1] contains data to be written to fd_.
  char buf_[kWritableFileBufferSize];
  size_t pos_;
  int fd_;

  const std::string filename_;
  // const std::string dirname_;  // The directory of filename_.
public:


Status PosixError(const std::string& context, int error_number) {
  if (error_number == ENOENT) {
    return Status::NotFound(context, strerror(error_number));
  } else {
    return Status::IOError(context, strerror(error_number));
  }
}


  //实际上的将Slice写入Log
 Status Append(const Slice& data) override {
   printf("The real write to the log(Append)...The data : %s\n", data.data());

   size_t write_size = data.size();         //需要写入的size
   const char* write_data = data.data();    //需要写入的指针的位置
   
   //尽可能的多写，判断需要写的 和 buf中剩余的空间
   size_t copy_size = std::min(write_size, kWritableFileBufferSize - pos_);
   std::memcpy(buf_ + pos_, write_data, copy_size);   //将内容写到缓冲区中

   //更新源数据的属性值
   write_data += copy_size;
   write_size -= copy_size;   

   //更新buf中pos_的位置
   pos_ += copy_size;   

   if (write_size == 0) {
     return Status::OK();
   }

   //走到这就代表buf中装不下.刷盘
   Status status = FlushBuffer();

   if (!status.ok()) {
     return status;
   }

   //刷盘之后，小数据写到buf,大数据直接写到日志
   if (write_size < kWritableFileBufferSize) {
     std::memcpy(buf_, write_data, write_size);
     pos_ = write_size;
     return Status::OK();
   }
   return WriteUnbuffered(write_data, write_size);
 }  
 

  Status Close() override {
    Status status = FlushBuffer();          //将剩余的buf刷盘
    const int close_result = close(fd_);    //关闭文件
    if (close_result < 0 && status.ok()) {
      status = PosixError(filename_, errno);
    }
    fd_ = -1;
    return status;
  }

  //将缓冲区的刷到磁盘中
  Status Flush() override {
    return FlushBuffer();     
  }
  //进行同步，因为是WAL，所以写完日志之后就将数据进行同步到磁盘上面
  Status Sync() override {

    //todo : SyncDirIfManifest
    Status status = FlushBuffer();
    if (!status.ok()) {
      return status;
    }

    //将同fd相关联的所有的缓冲刷新到磁盘上面
    return SyncFd(fd_, filename_);

  }

private:
  //将buf刷到日志
  Status FlushBuffer() {
    Status status = WriteUnbuffered(buf_, pos_);
    pos_ = 0;
    return status;
  }

  Status WriteUnbuffered(const char* data, size_t size) {
    //将数据写到文件中
    printf("具体的写到Log,fd中:%s, the fd : %d,\n", data, fd_);
    while (size > 0) {
      ssize_t write_result = write(fd_, data, size);
      if (write_result < 0) {
        if (errno == EINTR) {
          continue;         //Retry
        }
        return PosixError(filename_, errno);  //Error
      }

      data += write_result;
      size -= write_result;
    }
    return Status::OK();
  }

  //将同文件描述符号相关的所有的缓冲区的数据都持久化到磁盘上面,静态成员函数不能够在类内部进行设置
  static Status SyncFd(int fd, const std::string& fd_path) {
    bool sync_success = fsync(fd) == 0;     //将缓冲区的数据进行同步到磁盘上面,Return 0 if write ok 
    if (sync_success) {
      return Status::OK();
    }
    return PosixError(fd_path, errno);
  }


};


    
}



#endif