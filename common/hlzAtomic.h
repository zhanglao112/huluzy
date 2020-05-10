//
// Author: zhanglao ( nwlzee at gmail dot com)
//

#ifndef __HULUZY_COMMON_HLZATOMIC_H__
#define __HULUZY_COMMON_HLZATOMIC_H__

namespace huluzy {

typedef volatile int8_t	atomic_8_t;
typedef volatile int16_t atomic_16_t;
typedef volatile int32_t atomic_32_t;
typedef volatile int64_t aotmic_64_t;

#define ATOMIC_INCREASE(PTR) __sync_add_and_fetch(PTR,1)
#define ATOMIC_DECREASE(PTR) __sync_sub_and_fetch(PTR,1)
static inline int atomic_add(volatile int *count, int add) {
    __asm__ __volatile__(
        "lock xadd %0, (%1);"
        : "=a"(add)
        : "r"(count), "a"(add)
        : "memory");
    return add;
}
f
}

#endif
