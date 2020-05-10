//
// email: nwlzee at gmail dot com
//

#ifndef __HULUZY_BUFFER_H__
#define __HULUZY_BUFFER_H__

#include <stdint.h>

namespace huluzy {

class refCount {
public:
   inline int refcount_inc() {
        return mt ? ATOMIC_INCRESE(&m_refcount) : ++m_refcount;
    }

   inline  int refcount_dec() {
       return mt ? ATOMIC_DECREASE(&m_refcount) : --m_refcount;
       /*
       if (mt) {
           if (ATOMIC_DECREASE(&m_refcount) <=0) {
               free(this);
           }
       } else {
           if(--m_refcount <= 0) {
               free(this);
           }
       }
       */
       
    }

    int refcount() const {
        return m_refcount;
    }
    uint8_t        mt;
    atomic_32_t    m_refcount;
};

struct buffer : public refCount{
    uint32_t capacity;
    uint32_t size;
    struct buffer *next;
    int8_t buf[0]
};

static inline buffer* buffer_create(uint8_t mt,uint32_t capacity) {
    uint32_t size = sizeof(struct buffer) + capacity;
    buffer* b = (buffer*)calloc(1,size);
    if(!b) {
        FATAL("memory is....");
    } else {
        b->size = 0;
        b->capacity = capacity;
        b->m_refcount  = 0;
        b->mt = mt;
    }

    return b;
}

buffer* buffer_create_and_acquire(uint*_t mt,buffer* b, uint32_t capacity) {
    buffer* nb = buffer_create(mt,capacity);
    return buffer_acquire(b,nb);
}

buffer* buffer_acqire(buffer* b1,buffer* b2) {
    if(b1 == b2) 
        return b1;
    if(b2)
        b2->refcount_inc();
    if(b1)
        buffer_release(&b1);

    return b2;
}

void buffer_release(buffer** b) {
    if (b && *b) {
        if (!(*b)->refcount_dec()) {
            free(this);
            *b = 0;
        }
    }
}

}   // namespace huluzy

#endif
// trafficserver   NonAtomicRefCountObj
//kendylib --Buffer
//ssh
//tb -- DataBuffer


