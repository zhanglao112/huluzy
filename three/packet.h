//
// (nwlzee AT gmail DOT com)
//

#ifndef __HULUZY_PACKET_H__
#define __HULUZY_PACKET_H__

#include "buffer.h"

namespace huluzy {
class Packet {
 public:
    Packet(Buffer*,uint32_t,uint32_t);
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
    Buffer* get_head() const {
        return m_head;
    }
 private:
    bool _Write(int8_t* src, uint32_t size);
    bool _Read(int8_t* dst, uint32_t size);
    bool EnsureFree(uint32_t size);

    Buffer* m_head;
    Buffer* m_current;
    uint32_t pos_;
    uint32_t data_size;

};
}

#endif
