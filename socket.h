#ifndef __HULUZY_SOCKET_H__
#define __HULUZY_SOCKET_H__

#include "huluzy"

namespace huluzy {
class Socket {
public:
    Socket();
    ~Socket();
    void on_read_active();
    void on_write_active();
    static int32_t Process(Socket *);
    int32_t raw_send(st_io *io_req,int32_t *bytes_transfer,uint32_t *err_code);
    int32_t raw_recv(st_io *io_req,int32_t *bytes_transfer,uint32_t *err_code);
    
    void (*OnRread) (int32_t,st_io*);
    void (*OnWrite) (int32_t,st_io*);
private:
    inline void _recv();
    inline void _send();
private:
    volatile int32_t isactived_;
    volatile int32_t readable_;
    volatile int32_t writeable_;
    int32_t fd_;
    PollEngie *m_engine;
    std::queue<st_io*> *pending_send;
    std::queue<st_io*> *pending_recv;
    
 
};

}

#endif
