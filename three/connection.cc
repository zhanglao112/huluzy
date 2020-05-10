#include "connection.h"
#include "packet.h"

namespace huluzy {


#define MAX_TRANSFER_SIZE 65536
#define BUFFER_CAPACITY 8192

Connection::Connection(int sockfd, IOEventloop *loop):
    IOHandler(loop) {
    _sockfd = sockfd;
}

Connection::~Connection() {

}

void Connection::HandleRead() {

    Buffer* rbuf = newBuffer(BUFFER_CAPACITY);
    Buffer* head = rbuf;
    struct iovec riov[2];
    uint32_t max_size = MAX_TRANSFER_SIZE;
    while(max_size) {
        uint32_t more = max_size - BUFFER_CAPACITY;
        if (more) {
            rbuf->next = NewBuffer(BUFFER_CAPACITY);
            
        }
    }

    Buffer* rbuf = NewBuffer(MAX_TRANSFER_SIZE);
    struct iovec iov[1];
    iov.iov_base = rbuf->start;
    iov.iov_len = rbuf->capacity;
    uint32_t recv_bytes = m_socket->readFromSocket(_sockfd, &iov, 1);
    
    if (recv_bytes > 0) {
        
    }
    if (NULL == m_rBuf) {
        m_rBuf = NewBuffer(BUFFER_SIZE);
        m_unpackBuf = m_rBuf;
        rpos_ = unpackpos_ = 0;
    }

    assert(m_rBuf->capacity >= BUFFER_SIZE);
    
    size_t iovlen = m_rBuf->capacity - rpos_;
    Buffer* old_rBuf = m_rBuf;
    uint32_t old_rpos = rpos_;
    uint32_t old_free = iovlen;
    iovlen = iovlen > BUFFER_SIZE ? BUFFER_SIZE : iovlen;
    uint32_t need_more = BUFFER_SIZE - iovlen;
    uint32_t iovcnt = 1;
    iov_r[0].iov_base = m_rBuf->start + rpos_;
    iov_r[0].iov_len = iovlen;
    if (need_more) {
        iovcnt++;
        if (NULL == m_rBuf->next) {
            m_rBuf->next = NewBuffer(BUFFER_SIZE);
        }
        m_rBuf = m_rBuf->next;
        rpos_ = 0;
        iov_r[1].iov_base  = m_rBuf->start + rpos_;
        iov_r[1].iov_len = need_more;
    }
    uint32_t recv_bytes = m_socket.readFromSocket(_sockfd,&iov_r,iovcnt);
    if (recv_bytes < old_free) {
        m_rBuf = old_rBuf;
        rpos_ = old_rpos + recv_bytes;

    } else {
        rpos_ += recv_bytes;
        make_packet();

    }
    //    assert(rpos_ <= m_rbuf->capacity);
    //m_packetProcess->packetProcess();  // todo: queue in conn or packetprocess ?
    // todo event
    
}

uint32_t Connection::prepare_send() {
    uint32_t can_send_bytes = MAX_SEND_SIZE;
    uint32_t i = 0;
    uint32_t buffer_size = 0;
    Packet* p = wq_.head();
    while(p && i < MAX_BUF_COUNT && can_send_bytes > 0) {
        Buffer* b = p->get_head();
        uint32_t pos = pos_; // todo packet method return 
        buffer_size = p->data_size;
        while(b && buffer_size > 0 && can_send_bytes > 0) {
            uint32_t copy_size = b->size - pos;
            copy_size = can_send_bytes > copy_size ? copy_size : can_send_bytes;
            copy_size  = copy_size > buffer_size ? buffer_size : copy_size;
            wiov_[i].iov_base = b->start + pos;
            wiov_[i].iov_len = copy_size;
            buffer_size -= copy_size;
            can_send_bytes -= copy_size;
            i++;
            b = b->next;
            pos = 0;
            
        }
        if (can_send_bytes > 0) { 
            p = p->next;
        }
    }
    return i;
}

void Connection:update_w_queue(uint32_t send_bytes) {
    Packet *w;
    uint32_t size;
    while(send_bytes > 0) {
        w = wq_.pop();
        if (send_bytes > w->data_size) {
            send_bytes -= w->data_size;
            delete w;
        } else {
            Buffer* b = w->m_wbuf_head;

            while(send_bytes > 0) {
                assert(b);
                uint32_t size = b->size - w->begin_pos_;
                size = send_bytes > size ? size : send_bytes;
                w->data_size -= size;
                send_bytes -= size;
                w->begin_pos_ += size;
                if (w->begin_pos_ > b->size) {
                    w->begin_pos_ = 0;
                    w->m_wbuf_head = w->m_wbuf_head->next;
                }
            }
            wq_.push_front(w);
        }
    }
}
void Connection::HandleWrite() {
    uint32_t iovcnt = prepare_send();
    ssize_t send_bytes = m_socket.writeToSocket(sockfd_, wiov, iovcnt);
    update_w_queue();
}


void Connection::send(uint8_t* send_buf, uint32_t size ) {

    Packet* wpacket = new Packet(NULL,0,0);
    wpacket.writeBytes(send_buf,size);
    // todo is not raw
    wp_.push(wpacket);
}

void Connection::make_packet() {
    uint32_t pk_len = 0;
    uint32_t pk_total_len = 0;
    for(;;) {
        if (!raw_) {
            if (unpack_size <= sizeof (pk_len)) break;
            pk_len = Buffer_Read(m_unpackbuf, unpackpos_, &pk_len, sizeof(pk_len));
            pk_total_len = pk_len + sizeof(pk_len);
            if (unpack_size < pk_total_len)  break;
            Packet *np = new Packet(m_unpackbuf,unpackpos_, pk_len);
            while(pk_total_len) {
                uint32_t size = m_unpackbuf->size - unpackpos_;
                size = pk_total_len > size ? size : pk_total_len;
                pk_total_len -= size;
                unpacksize_ -= size;
                unpackpos_ += size;
                if (unpackpos_ > m_unpackbuf->capacity) {
                    assert(m_unpackbuf->next);
                    unpackpos_ = 0;
                    m_unpackbuf = m_unpackbuf->next;
                }
                
            }
            
        } else {
            pk_len = m_unpackbuf->size - unpackpos_;
            if(!pk_len) break;
            Packet *np = new Packet(m_unpackbuf,unpackpos_, pk_len, this);
            unpacksize_ -= pk_len;
            unpackpos_ += pk_len;
            if(unpackpos_ >= m_unpackbuf->capacity) {
                assert(m_unpackbuf->next);
                unpackpos_ = 0;
                m_unpackbuf = m_unpackbuf->next;
            }
            
        }
        m_packetProcess(np, this);
    }
}


}
