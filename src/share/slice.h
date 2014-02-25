#ifndef _YOHUB_SHARE_SLICE_H_
#define _YOHUB_SHARE_SLICE_H_

#include <string.h>
#include <assert.h>
#include <string>

namespace yohub {

class Slice {
  public:
    Slice() : data_(""), size_(0) { }
    Slice(const char* d, size_t n) : data_(d), size_(n) { }
    Slice(const std::string& s) : data_(s.data()), size_(s.size()) { }

    const char* data() const { return data_; }
    size_t size() const { return size_; }
    void clear() { data_= ""; size_ = 0; }

    std::string ToString() const { return std::string(data_, size_); }

  private:
    const char* data_;
    size_t size_;
};

} // namespace yohub

#endif // _YOHUB_SHARE_SLICE_H_
