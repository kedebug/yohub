#ifndef _YOHUB_SHARE_ATOMIC_H_
#define _YOHUB_SHARE_ATOMIC_H_

#include <stdint.h>

namespace yohub {

// return old value
static inline int atomic_add(volatile int* count, int add) {
    __asm__ __volatile__(
        "lock xadd %0, (%1);"
        : "=a"(add)
        : "r"(count), "a"(add)
        : "memory");
    return add;
}

// return old value
static inline int atomic_swap(volatile void* lockword, int value) {
    __asm__ __volatile__(
        "lock xchg %0, (%1);"
        : "=a"(value)
        : "r"(lockword), "a"(value)
        : "memory");
    return value;
}

// return old value
static inline int atomic_comp_swap(volatile void* lockword,
                                   int exchange,
                                   int comperand) {
    __asm__ __volatile__(
        "lock cmpxchg %1, (%2);"
        : "=a"(comperand)
        : "d"(exchange), "r"(lockword), "a"(comperand));
    return comperand;
}

#define AtomicInc(x)            (atomic_add(&(x), 1) + 1)
#define AtomicDec(x)            (atomic_add(&(x), -1) - 1)
#define AtomicGetValue(x)       (atomic_comp_swap(&(x), 0, 0))
#define AtomicSetValue(x, v)    (atomic_swap(&(x), (v)))

} // namespace yohub

#endif // _YOHUB_SHARE_ATOMIC_H_
