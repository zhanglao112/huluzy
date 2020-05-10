#include "packet.h"

using namespace huluzy;

Packet::Packet(bool mt, 
               buffer* b,
               uint32_t pos,
               uint32_t pk_len,
               bool is_raw) {
    next_ = NULL;
    buf_ = buffer_acquire(NULL,b);
    len_ = pk_len;
    dataremain_ = len;
    pos_ = is_raw ? pos : ((pos+ sizeof(len))%buf->capacity);
    raw_ = is_raw;
    binbuf_ = NULL;
    binbufpos_ = 0;
    mt_  = mt;
    
}

Packet::Packet(Packet& other) {
    buf_  = buffer_acquire(NULL,other->buf);
    len_ = other.en_;
    dataremain_ = other.len;
    next = NULL;
    pos_ = other.pos_;
    raw_ = other.raw_;
    binbuf = NULL;
    binbufpos = 0;
    mt_ = other.mt_;
}

Packet::~Packet() {
    buffer_release(buf_);
    buffer_release(binbuf_);
    next_ = NULL;
}

int Packet::packet_read(int8_t *out,uint32_t size) {
    if (dataremain_ < size) {
        return -1;
    }
    while(size > 0) {
        uint32_t copy_size = buf->size -pos_;
        copy_size = copy_size >= size ? size : copy_size;
        memcpy(out,buf+pos_,copy_size);
        size -= copy_size;
        pos_ += copy_size;
        dataremain_ -= copy_size;
        out += copy_size;
        if (pos_ >= buf_->size && dataremain) {
            pos_ = 0;
            buf_ = buffer_acquire(buf_,buf_->next);
        }
    }
    return 0;
}

uint8_t Packet::packet_read_uint8() {
    uint8_t value = 0;
    packet_read((int8_t*)&value,sizeof (value));
    return value;
}

uint16_t Packet::packet_read_uint16() {
    uint16_t value = 0;
    packet_read((int8_t*)&value, sizeof(value));
    return value;
}

uint32_t Packet::packet_read_uint32() {
    uint32_t value  = 0;
    packet_read((int8_t*)&value, sizeof(value));
    return value;
}

uint64_t Packet::packet_read_uint64() {
    uint64_t value  = 0;
    packet_read((int8_t*)&value, sizeof(value));
    return value;
}

double Packet::packet_read_double() {
    double value = 0;
    packet_read((int8_t*)&value, sizeof(value));
    return value;
}


const void* Packet::packet_raw_read_binary(uint32_t *len) {
    void *addr = NULL;
    uint32_t size = 0;
    if (!dataremain_)
        return addr;
    if (buf_->size - pos_ >= dataremain_) {
        *len = dataremain_;
        dataremain_  = 0;
        addr = &buf_[pos_];
        pos_ += dataremain_; // something wrong?
    } else {
        if (!binbuf) {
            binbufpos_ = 0;
            binbuf_ = buffer_create_and_acquire(mt_,NULL,len_);
        }
        addr = binbuf_  + binbufpos_;
        size = dataremain_;
        while(size) {
            uint32_t copy_size = buf_->size  - pos_;
            copy_size  = copy_size >= size ? size : copy_size;
            memcpy(binbuf_+binbufpos_,buf_+pos_,copy_size);
            size - = copy_size;
            pos_ += copy_size;
            dataremain_ -= copy_size;
            binbufpos_ += copy_size;
            if(pos_ >= buf_->size && dataremain_) {
                pos_ = 0;
                buf_ = buffer_acquire(buf_,buf_->next);
            }
        }
    }
    return addr;
}

const char* Packet::packet_read_string() {
    uint32_t len = 0;
    if(raw_)
        return packet_raw_read_binary(&len);
    return (const char*)packet_read_binary(&len);
}

const void* Packet::packet_read_binary(uint32_t *len) {
    void *addr = 0;
    uint32_t size = 0;
    if(raw_) {
        return packet_raw_read_binary(len);
    }
    size = packet_read_uint32();
    *len = size;
    if(!dataremain || dataremain < size) {
        return addr;
    }
    if(buf_->size - pos_ >= size) {
        addr = &buf_>buf[pos_];
        pos_ += size;
        dataremain_ -= size;
        if (pos_ >= buf->size && dataremain) {
            pos_ = 0;
            buf_ = buffer_acquire(buf_,buf_->next);
        }
    } else {
        if(!binbuf_) {
            binbufpos_ = 0;
            binbuf_ = buffer_create_acquire(mt_,NULL,len_);
        }
        addr = binbuf_->buf + binbufpos_;
        while(size) {
            uint32_t copy_size = buf_->size - pos_;
            copy_size = copy_size >= size ? size : copy_size;
            memcpy(binbuf_->buf+binbufpos_,buf_->buf+pos_,copy_size);
            size -= copy_size;
            pos_ += copy_size;
            dataremain_ -= copy_size;
            binbufpos_ += copy_size;
            if(pos_ >= buf_->size && dataremian_) {
                pos_ = 0;
                buf_ = buffer_acquire(buf_,buf_->next);
            }
        }
    }
    return addr;
}
