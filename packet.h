//
// ( nwlzee at gmail dot com)
//

#ifndef __HULUZY_PACKET_H__
#define __HULUZY_PACKET_H__

#include "buffer.h"

namespace hulyzy {
class Packet {
public:
    Packet* next_;
    Packet(buffer*,uint32_t,uint32_t,bool);
    Packet(Packet&);
    ~Packet();
    inline uint32_t packet_len() const {
        return len_;
    }
    inline uint32_t packet_datarmain() const {
        return dataremain_;
    }
    
   uint8_t packet_read_uint8();
   uint16_t packet_read_uint16();
   uint32_t packet_read_uint32();
   uint64_t packet_read_uint64();
   const char* packet_read_string();
   const void* packet_read_binary(uint32_t *len);

 private:
    int packet_read(int8_t *out, uint32_t size);
    const void* packet_raw_read_binary(uint32_t *len);
    

private:
    buffer* buf_;
    uint32_t len_;
    uint32_t dataremain_;
    uint32_t pos_;
    bool raw_;
    buffer* binbuf_;
    uint32_t binbufpos_;
    bool mt_;
    
};

}

#endif
