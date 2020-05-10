#include "connection.h"

using namespace huluzy;

Connection::Connection(HANDLE s,
                       bool is_raw,
                       bool mt,
                       process_packet _process_packet,
                       on_disconnect _on_disconnect) {
    socket = s;
    _process_packet = _process_packet;
    _on_disconnect = _on_disconnect;
    next_recv_buf = NULL;
    next_recv_pos = 0;
    unpack_buf = NULL;
    unpack_pos = 0;
    unpack_size = 0;
    recv_overlap.c = this;
    send_overlap.c = this;
    raw = is_raw;
    mt = mt;
    is_close = 0;
}

Connection::~Connection() {
    if(!recv_overlap.isUsed && !send_overlap.isUsed) {
        packet p;
        while(p = send_list.pop()) {
            //free,buffer_realse

        }
        buffer_release(unpack_buf);
        buffer_release(next_recv_buf);

        printf("connection_destroy\n");
    }
    // ::close(socket);
}


inline Connection::update_next_recv_pos(int32_t bytestransfer) {
    uint32_t size;
    while(bytestransfer) {
        size = next_recv_buf->capacity - next_recv_pos;
        size = size > (uint32_t)bytestransfer ? (uint32_t)bytestransfer : size;
        next_recv_buf->size += size;
        next_recv_pos += size;
        bytestransfer -= size;
        if(next_recv_pos >= next_recv_buf->capacity) {
            if(!next_recv_buf->next)
                next_recv_buf->next = buffer_create_and_acqure(mt,NULL,BUFFER_SIZE);
            next_recv_buf = buffer_acquire(next_recv_buf,next_recv_buf->next);
            next_recv_pos = 0;
        }
    }
}

inline void Connection::unpack() {
    uint32_t pk_len = 0;
    uint32_t pk_total_size;
    packet *rp;
    for(;;) {
        if(!raw) {
            if(unpack_size <= sizeof(uint32_t))
                break;
            buffer_read(unpack_buf,unpack_pos,(int8_t*)&pk_len,sizeof(pk_len));
            pk_total_size = pk_len + sizeof(pk_len);
            if(pk_total_size > unpack_size)
                break;
            rp = new Packet(mt,unpack_buf,unpack_pos,pk_eln,raw);
            while(pk_total_size) {
                uint32_t size = unpack_buf->size - unpack_pos;
                size = pk_total_size > size ? size : pk_total_size;
                unpack_pos += size;
                pk_total_size -= size;
                unpack_size -= size;
                if( unpack_pos >= unpack_buf->capacity) {
                    assert(unpack_buf->next);
                    unpack_pos = 0;
                    unpack_buf = buffer_acquire(unapck_buf,unpack_buf->next);
                }
            }
        } else {
            pk_len = unpack_buf->size - unpack_pos;
            if(!pk_len)
                return;
            rp = new Packet(mt,NULL,unpack_buf,unpack_pos,pk_len,raw);
            unpack_pos += pk_len;
            unpack_size -= pk_len;
            if(unpack_pos >= unpack_buf->capacity) {
                assert(unpack_buf->next);
                unpack_pos = 0;
                unpack_buf = buffer_acquire(unpack_buf,unpack_buf->next);
            }
        }
        _process_packet(this,rp);
    }
}

void Connection::RecvFinish(int32_t bytestransfer,st_io *io) {
    struct OVERLAPCONTEXT *OVERLAP = (struct OVERLAPCONTEXT*)io;
    Connection *c = OVERLAP->c;
    Packet r;
    uint32_t recv_size;
    uint32_t free_buffer_size;
    buffer *buf;
    uint32_t pos;
    int32_t i = 0;
    uint32_t err_code = io->err_code;
    for(;;) {
        if(bytestransfer == 0 || (bytestransfer < 0 && err_code != EAGINA)) {
            c->recv_overlap.isUsed = 0;
            if(!c->send_overlap.isUsed) {
                if(c->is_close == 1) {
                    printf("RecvFinish is_close\n");
                    exit(0);
                }
                // -1,passive close
                c->_on_disconect(c,-1);
            }
            break;
        } else if (bytestransfer < 0 && err_code == EAGAIN) {
            break;
        } else {
            int32_t total_size = 0;
            int32_t flag  = RECV_NOW;
            while(bytestransfer > 0) {
                c->last_recv = GetCurrentMs();
                updata_next_recv_pos(c,bytestransfer);
                c->unpack_size += bytestransfer;
                total_size += bytestransfer;
                unpack(c);
                buf = c->next_recv_buf;
                pos = c->next_recv_pos;
                recv_size = BUFFER_SIZE;
                while(recv_szie) {
                    free_buffer_size = buf->capacity - pso;
                    free_buffer_size = recv_size > free_buffer_size ? free_buffer_size : recv_size;
                    c->wrecvbuf[i].iov_len = free_buffer_size;
                    c->wrecvbuf[i].iov_base = buf->buf+pos;
                    recv_size -= free_buffer_size;
                    pos += free_buffer_size;
                    if(recv_size && pos >= buf->capacity) {
                        pos = 0;
                        if(!buf->next)
                            buf->next = buffer_create_and_acquire(c->mt,NULL,BUFFER_SIZE);
                        buf = buf->next;
                    }
                    ++i;
                }
                c->recv_overlap.isUsed = 1;
                c->recv_overlap_m_super.iovec_count = i;
                c_>recv_overlap_m_super.iovec = c->wrecvbuf;
                if(total_size > 65535)
                    flag = RECV_POST;
                bytestransfer = WSARecv(c->socket,&c->recv_overlap.m_super,flag,&err_code);
            }
        }
    }
}

inline st_io Connection::prepare_send() {
    int32_t i = 0;
    Packet *w = (Packet*)send_list.top();
    buffer* b;
    uint32_t pos;
    st_io *O = NULL;
    uint32_t buffer_size = 0;
    uint32_t size = 0;
    uint32_t send_size_remain = MAX_SEND_SIZE;
    while(w && i < MAX_WBUF && send_size_remain > 0) {
        pos = w->begin_pos;
        b = w->buf;
        buffer_size  = w->data_size;
        while(i< MAX_WBAF && b && buffer_size && send_size_remain > 0){
            wsendbuf[i].iov_bae = b->buf + pos;
            size = b->size -pos;
            size = size > buffer_size ? buffer_size : size;
            size  = size > send_size_remain ? send_size_remain : size;
            buffer_size -= size;
            send_size_remain -= size;
            wsendbuf[i].iov_len = size;
            ++i;
            b = b->next;
            pos = 0;
        }
        if (send_size_remain > 0)
            w = (Packet*)w->next;
    }
    if(i) {
        send_overlap.m_super.iovec_count = i;
        send_overlap.m_super.iovec = wsendbuf;
        O = (st_io*)&send_overlap;
    }
    return O;
}

inline void Connection::update_send_list(int32_t bytestransfer) {
    Packet w;
    uint32_t size;
    while(bytestransfer) {
        w = send_list.pop();
        assert(w);
        if((uint32_t)bytestransfer >= w->data_size) {
            // 一个包发完
            bytestransfer -= w->data_size;
            if(w->_packet_send_finish)
                w->_packet_send_finish(this,w);
            packet_destroy(&w);
        } else {
            while(bytestransfer) {
                size = w->buf->size - w->begin_pos;
                size = size > (uint32_t)bytestransfer ? (uint32_t)bytestransfer:size;
                bytestransfer -= size;
                w->begin_pos += size;
                w->data_size -= size;
                if(w->begin_pos >= w->buf->size) {
                    w->begin_pos = 0;
                    w->buf = buffer_acquire(w->buf,w->buf->next);
                }
            }
            send_list.push_front(w);
        }
    }
}

int32_t Connection::connection_send(wpacket_t w,packet_send_finish callback) {
    int32_t bytestransfer = 0;
    uint32_t err_code = 0;
    st_io *O;
    int32_t ret = 1;
    if(w) {
        w->send_tick = GetCurrentMs();
        w->_packet_send_finish = callback;
        LINK_LIST_PUSH_BACK(c->send_list,w);
    }
    if(!c->send_overlap.isUsed) {
        O = prepare_send(c);
        if(O) {
            c->send_overlap.isUsed = 1;
            return WSASend(c->socket,O,SEND_POST,&err_code);
        }
    }
    return -1;
}

void Connection::connection_push_packet(wpacket_t w,packet_send_finish callback) {
    if(w)
    {
        w->_packet_send_finish = callback;
        LINK_LIST_PUSH_BACK(c->send_list,w);
    }
}

void Connection::SendFinish(int32_t bytestransfer,st_io *io) {
    struct OVERLAPCONTEXT *OVERLAP = (struct OVERLAPCONTEXT *)io;
    struct connection *c = OVERLAP->c;
    uint32_t err_code = io->err_code;
    for(;;) {
        if(bytestransfer == 0 || (bytestransfer < 0 && err_code != EAGAIN)) {
            c->send_overlap.isUsed = 0;
            if(!c->recv_overlap.isUsed) {
                if(c->is_close == 1) {
                    printf("SendFinish is_close\n");
                    exit(0);
                }
                //-1,passive close
                c->_on_disconnect(c,-1);
            }
            break;
        } else if(bytestransfer < 0 && err_code == EAGAIN) {
            break;
          } else {
            while(bytestransfer > 0) {
                update_send_list(c,bytestransfer);
                io = prepare_send(c);
                if(!io) {
                    //没有数据需要发送了
                    c->send_overlap.isUsed = 0;
                    return;
                }
                bytestransfer = WSASend(c->socket,io,SEND_NOW,&err_code);
            }
        }
    }
}


uint32_t Connection::connection_start_recv() {
    if(unpack_buf)
        return -1;
    unpack_buf = buffer_create_and_acquire(mt,NULL,BUFFER_SIZE);
    next_recv_buf = buffer_acquire(NULL,unpack_buf);
    next_recv_pos = unpack_pos = unpack_size = 0;
    wrecvbuf[0].iov_len = BUFFER_SIZE;
    wrecvbuf[0].iov_base = next_recv_buf->buf;
    recv_overlap.m_super.iovec_count = 1;
    recv_overlap.isUsed = 1;
    recv_overlap.m_super.iovec = wrecvbuf;
    uint32_t err_code;
    return WSARecv(socket,&recv_overlap.m_super,0,&err_code);
}

void Conenction::connection_active_close() {
    ::close(socket);
    recv_overlap.isUsed = send_overlap.isUsed = 0;
    if(_on_disconnect)
        _on_disconnect(c,-2); // -2,active close
}

