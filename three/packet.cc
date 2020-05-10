#incldue "packet.h"
namespace huluzy {
Packet::Packet(Buffer* recvbuf,uint32_t pos,uint32_t len) {
    m_head = recvbuf;
    m_current = recvbuf;
    pos_ = pos;
    data_size_ = len;
}

Packet::~Packet() {
    // todo buffer free
}

bool Packet::EnsureFree(uint32_t want_size) {
    if ( NULL == m_current) {
        m_current = NewBuffer(want_size);
        if(!m_head) {
            FATAL("memory out of service");
            return false;
        }
        m_head = m_head ? m_head : m_current;
        return true;
    }
    Buffer* tmp_buf = m_current;
    uint32_t can_copy = tmp_buf->capacity - pos_;
    want_size -= can_copy > want_size : want_size ? can_copy;
    while(want_size) {
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
        copy_size = m_current->capacity - pos_;
        copy_size = copy_size > size ? size : copy_size;
        memcpy(m_current->start+pos_, src, copy_size);
        src += copy_size;
        data_size += copy_size;
        size -= copy_size;
        pos_ += copy_size;
        if (pos_ >= m_current->capacity) {
            assert(m_current->next);
            m_current = m_current->next;
            pos_ = 0;
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
    if (NULL == m_head || data_size_ < size) return false;
    while(size) {
        uint32_t copy_size = m_current->size - pos_;
        copy_size = copy_size > size ? size : copy_size;
        memcpy(dst,m_current->start+pos_,copy_size);
        dst += copy_size;  // todo: reinterpret_cast<uintptr_t>(out) + copy_size
        size -= copy_size;
        data_size_ -= copy_size;
        pos_ += copy_size;
        if (pos_ >= m_current->size) {
            assert(m_current->next);
            pos_ = 0;
            m_current = m_current->next;
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

