//
// (nwlzee AT gmail DOT com)
//

#ifndef __HULUZY_DATABUFFER_H__
#define __HULUZY_DATABUFFER_H__

#include "span.h"

namespace huluzy {
class DataBuffer {
 public:
    DataBuffer();
    ~DataBuffer();
    void writeInt8(uint8_t n) {
        
    }
 private:
    Span *m_span;
    int spanCapacity_;
};

}

#endif
