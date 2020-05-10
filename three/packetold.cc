#incldue "packet.h"

namespace huluzy {
Packet::Packet() {
    m_curWbuf = NULL;
    wpos_ = 0;
    data_size_ = 0;
}

Packet::~Packet() {

}

bool Packet::EnsureFree(uint32_t want_size) {
    if ( NULL == m_curWBuf) {
        m_curWBuf = NewBuffer(want_size);
        if(!m_curWBuf) {
            FATAL("memory out of service");
            return false;
        }
        return true;
    }
    Buffer* tmp_buf = m_curWBuf;
    uint32_t can_copy = tmp_buf->capacity - bpos_;
    want_size -= can_copy > want_size : want_size ? can_copy;
    while(want_size ) {
        if (!tmp_buf->next) {
            tmp_buf->next = NewBuffer(tmp_buf->capacity);
            if (!tmp_buf->next) {
                FATAL("memory out of service");
                return false;
            }
        }
        want_size -= tmp_buf->capacity > want_size ? want_size : tmp_buf->capacity;
        tmp_buf = tmp_buf->next;
    }
    return true;
}

bool Packet::_Write(int8_t* src, uint32_t size) {
    if (!EnsureFree(size)){
        return false;
    }
    uint32_t copy_size = 0;
    while(size) {
        copy_size = m_curWBuf->capacity - bpos_;
        copy_size = copy_size > size ? size : copy_size;
        memcpy(m_currentBuf->start+bpos_, src, copy_size);
        src += copy_size;
        size -= copy_size;
        bpos_ += copy_size;
        if (bpos_ >= m_curWbuf->capacity) {
            assert(m_curWBuf->next);
            m_curWBuf = m_curWBuf->next;
            bpos_ = 0;
        }
    }

    return true;
}

void Packet::writeInt8(uint8_t n) {
    _Write((uint8_t*)&n, sizeof(uint8_t));
}

void Packet::writeInt16(uint16_t n) {
    _Write((uint8_t*)&n, sizeof(uint16_t));
}

void Packet::writeInt32(uint32_t n) {
    _Write((uint32_t*)&n, sizeof(uint32_t));
}

void Packet::writeInt64(uint32_t n) {
    _Write((uint8_t*)&n, sizeof(uint64_t));
}


void Packet::writeBytes(const void* src, int len) {
    // todo raw write
    _Write((uint8_t*)src,len);
}

void Packet::writeString(const char* str) {
    int len = (str ? strlen(str) : 0);
    if (len > 0) len++;
    writeBytes(str,len);
}

void Packet::writeString(const std::string& str) {
    writeString(str.c_str());
}


bool Packet::_Read(int8_t* dst,uint32_t size) {
    if (NULL == m_curRBuf || data_size_ < size) return false;
    while(size) {
        uint32_t copy_size = m_curRBuf->size - rpos_;
        copy_size = copy_size > size ? size : copy_size;
        memcpy(dst,m_curRBuf->start,copy_size);
        dst += copy_size;  // todo: reinterpret_cast<uintptr_t>(out) + copy_size
        size -= copy_size;
        data_size_ -= copy_size;
        rpos_ += copy_size;
        if (rpos_ >= m_curRbuf->size) {
            rpos_ = 0;
            m_curRBuf = m_curRBuf->next;
        }
    }
    return true;
}
uint8_t Packet::readInt8() {
    uint8_t result = 0;
    bool success = _Read((uint8_t*)&result,sizeof(uint8_t));
    if(!success) {
        FATAL("Packet::readInt8,the packet didn't have so many bytes to read");
    } 
    return result;
}

uint16_t Packet::readInt16() {
    uint16_t result = 0;
    bool success = _Read((uint8_t*)&result,sizeof(uint16_t));
    if(!success) {
        FATAL("Packet::readInt16,the packet didn't have so many bytes to read");
    } 
    return result;
}

uint32_t Packet::readInt32() {
    uint32_t result = 0;
    bool success = _Read((uint8_t*)&result,sizeof(uint32_t));
    if(!success) {
        FATAL("Packet::readInt32,the packet didn't have so many bytes to read");
    } 
    return result;
}

uint64_t Packet::readInt64() {
    uint64_t result = 0;
    bool success = _Read((uint8_t*)&result,sizeof(uint64_t));
    if(!success) {
        FATAL("Packet::readInt64,the packet didn't have so many bytes to read");
    } 
    return result;
}

bool Packet::readBytes(void *dst, uint32_t len) {
    if (!dst || data_size < len) return false;

    _Read((uint8_t*)dst,len);
    return true;
}



}

