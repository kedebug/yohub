#include "network/buffer.h"
#include "share/log.h"
#include <errno.h>

using namespace yohub;

Buffer::Buffer() 
    : mem_(1024),
      reader_idx_(0),
      writer_idx_(0)
{ }

int Buffer::ReadFd(int fd, bool* ok) {
    char reserve[65536];
    int total = 0;

    while (true) {
        int result = ::read(fd, reserve, sizeof(reserve));

        if (result == -1) {
            *ok = false;
            if (errno == EAGAIN) {
                return total;
            } else {
                LOG_WARN("read error occur: %s", strerror(errno));
                return result;
            }
        } else if (result == 0) {
            if (total > 0) {
                *ok = true;
                return total;
            } else {
                *ok = false;
                return 0;
            }
        } else {
            assert(result > 0);
            total += result;
            Append(reserve, result);
        }
    }
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
