#include "socket.h"

using namespace huluzy;

Socket::Socket(int32_t fd, PollEngie *engine):
    fd_(fd),
    m_engine(engine)
{
    isactived_ = 0;
}

Socket::~Socket {

}


void Socket::on_read_active() {
    readable_ = 1;
    if(!isactived_ && !pending_recv->empty()) {
        isactived_  = 1;
        engine->actived.push_back(this);
    }
}

void Socket::on_write_active() {
    writeable_ = 1;
    if (!isactived && !pending_send->empty()) {
        isactived_ = 1;
        engine->actived.push_back(this);
    }
}

int32_t Socket::raw_recv(st_io *io_req,int32_t *bytes_transfer,uint32_t *err_code) {
    int32_t ret = 0;
    *err_code = 0;
    *bytes_transfer = readv(fd_,io_req->iovec,io_req->iovec_count);
    if(*bytes_transfer < 0){
        if (errno == EAGAIN) {
            readable_ = 0;
            pending_recv.push_front(io_req);
        }
        *err_code = errno;
        ret = -1;
    } else if (*bytes_transfer == 0) 
        ret = 0;
    else 
        ret = *bytes_transfer;

    return ret;
}

inline void Socket:: _recv() { 
    int32_t ret =  -1;
    int32_t bytes_transfer = 0;
    st_io* io_req = 0;
    if(readable_) {
        if(io_req = pending_recv.pop) {
            ret = raw_recv(io_req,&bytes_transfer,&io_req->err_code);
            if (ret >= 0 || io_req->err_code != EAGAIN)
                onRead(bytes_transfer,io_req);
        }
    }
} 

int32_t Socket::raw_send(st_io *io_req,int32_t *bytes_transfer,uint32_t *err_code) {
    int32_t ret = 0;
    *err_code = 0;
    *bytes_transfer = writev(fd_,io_req->iovec,io_req->iovec_count);
    if(*bytes_tranfer < 0) {
        if (errno == EAGAIN) {
            writeable_ = 0;
            pending_send.push_front(io_req);
        }
        ret = -1;
        *err_code  = errno;
    } else if (*bytes_transfer == 0)
        ret  =0;
    else 
        ret = *bytes_transfer;
    return ret;
}

inline void Socket::_send() {
    int32_t ret = -1;
    int32_t bytes_transfer = 0;
    st_io *io_req = 0;
    if(writeable_) {
        if(io_req = pending_send.pop()) {
            ret = raw_send(io_req,&bytes_transfer,&io_req->err_code);
            if(ret >= 0 || io_req->err_code != EAGAIN) 
                OnWrite(bytes_transfer,io_req);
        }
    }
    
}

int32_t Socket::Process(Socket *s) {
    s->_recv();
    s->_send();
    int32_t read_active = s->readable_ && !s->pending_recv.empty();
    int32_t write_active = s->writeable_ && !s->pending_send.empty();
    return (read_active || write_active ) && s->isactived == 0;
}
