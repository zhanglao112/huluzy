//
// (nwlzee AT gmail DOT com)
//

#ifndef __HULUZY_PACKET_H__
#define __HULUZY_PACKET_H__

#include "buffer.h"

namespace huluzy {
class Packet {
 public:
    Packet();
    ~Packet();
    void writeInt8(uint8_t);
    void writeInt16(uint16_t);
    void writeInt32(uint32_t);
    void writeInt64(uint64_t);
    void writeBytes(const void*, uint32_t);
    void writeString(const char*);
    void writeString(const std::stirng&);
    template<typename T>
    void writeType(T n) {
        _Write((uint8_t*)&n, sizeof(T));
    }
    
    
    uint8_t readInt8();
    uint16_t readInt16();
    uint32_t readInt32();
    uint64_t readInt64();
    boo readBytes(void*, uint32_t);
    template<typename T>
    bool readType(T* o) {
        if(!o || data_size < sizeof(T)) return false;
        _Read((uint8_t*)o, sizeof(T));
        return true;
    }
 private:
    bool _Write(int8_t* src, uint32_t size);
    bool _Read(int8_t* dst, uint32_t size);
    bool EnsureFree(uint32_t size);
    struct DLinkBuffer {
        Buffer* head_;
        Buffer* tail_;
    };
    DLinkBuffer* m_buffer;

        union {
            Buffer* m_curWBuf;
            Buffer* m_curRBuf;
        };
        union {
            uint32_t wpos_;
            uint32_t rpos_;
        };
        Buffer* m_wbuf_head;
        uint32_t begin_pos_;
        uint32_t data_size_;
        /*
    Buffer* m_curWBuf;
    Buffer* m_curRBuf;
    uint32_t wpos_;
    uint32_t rpos_;
        */
};
}

#endif
