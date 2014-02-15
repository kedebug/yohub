#include "network/io_event.h"

using namespace yohub;

void IoEvent::AttachChannel(Channel* channel) {
    epoller_->AttachChannel(channel);
}

void IoEvent::DetachChannel(Channel* channel) {
    epoller_->DetachChannel(channel);
}
