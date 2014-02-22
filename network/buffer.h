#ifndef _YOHUB_NETWORK_BUFFER_H_
#define _YOHUB_NETWORK_BUFFER_H_

#include <string>
#include <vector>
#include "share/slice.h"

namespace yohub {

class Buffer {
  public:
    Buffer();

    int ReadFd(int fd, bool* ok);

    void Append(const char* data, size_t bytes);

    void ReadableForward(size_t n) {
        assert(reader_idx_ + n < writer_idx_);
        reader_idx_ += n;
    }

    size_t ReadableBytes() const {
        return writer_idx_ - reader_idx_;
    }

    size_t WritableBytes() const {
        return mem_.size() - writer_idx_;
    }

    std::string TakeAsString() {
        std::string result(begin() + reader_idx_, ReadableBytes());
        reader_idx_ = 0;
        writer_idx_ = 0;
        return result;
    }

    const Slice ToSlice() const {
        return Slice(begin() + reader_idx_, ReadableBytes());
    }

  private:
    void EnsureWritableBytes(size_t require);

    char* begin() { return &*mem_.begin(); }
    const char* begin() const { return &*mem_.begin(); }

    std::vector<char> mem_;
    size_t reader_idx_;
    size_t writer_idx_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_BUFFER_H_
