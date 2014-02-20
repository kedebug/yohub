#include "network/buffer.h"
#include "share/log.h"
#include <errno.h>
#include <sys/uio.h>

using namespace yohub;

Buffer::Buffer() 
    : mem_(8),
      reader_idx_(0),
      writer_idx_(0)
{ }

int Buffer::ReadFd(int fd) {
    char reserve[65536];
    struct iovec iov[2];
    size_t space = WritableBytes();

    iov[0].iov_base = begin() + writer_idx_;
    iov[0].iov_len = space;
    iov[1].iov_base = reserve;
    iov[1].iov_len = sizeof(reserve);

    int result = ::readv(fd, iov, 2);
    if (result < 0) {
        LOG_WARN("readv error: %s", strerror(errno));
    } else if (result <= static_cast<int>(space)) {
        writer_idx_ += result;
    } else {
        writer_idx_ = mem_.size();
        Append(reserve, result - space);
    }

    return result;
}

void Buffer::Append(const char* data, size_t size) {
    EnsureWritableBytes(size);
    std::copy(data, data + size, begin() + writer_idx_);
    writer_idx_ += size;
}

void Buffer::EnsureWritableBytes(size_t require) {
    if (WritableBytes() >= require) {
        return;
    }

    if (reader_idx_ + WritableBytes() >= require) {
        int temp = ReadableBytes();
        std::copy(begin() + reader_idx_, begin() + writer_idx_, begin());
        reader_idx_ = 0;
        writer_idx_ = temp;
    } else {
        mem_.resize(writer_idx_ + require);
    }
}
