#ifndef __HULUZY_CONNECTION_H__
#define __HULUZY_CONNECTION_H__

class Connection;
struct OVERLAPCONTEXT {
    st_io m_super;
    bool isUsed;
    Connection *c;
};

typedef void (*process_packet)(struct connnection*,packet*);
typedef void (*on_disconnect)(struct connection*,int32_t reason);
#deifne MAX_WBAF 512
#define MAX_SEND_SIZE 4096

namespace huluzy {
class Connection {

 public:
    Connection(HANDLE,bool,bool,process_packet,on_disconnect);
    ~Connection();
    int32_t connection_start_recv();
    void connection_active_close();
    void RecvFinish(int32_t bytestransfer,st_io *io);
    void SendFinish(int32_t bytestransfer,st_io *io);
    int32_t connection_send(Packet* w,pack_send_finish callback);
    void connection_push_packet(Packet *w,packet_send_finish callback);
    void SendFinish(int32_t bytestransfer,st_io *io);
 private:
    inline void undate_next_recv_pos(int32_t bytestransfer);
    inline void unpack();
    inline st_io *prepare_send();
    inline void update_send_list(int32_t bytestransfer);

    HANDLE socket;
    
    struct iovec wsendbuf[MAX_WBAF];
    struct iovec wrecvbuf[2];

    struct OVERLAPCONTEXT send_overlap;
    struct OVERLAPCONTEXT recv_overlap;
    uint32_t unpack_size;

    uint32_t unpack_pos;
    uint32_t next_recv_pos;

    buffer* next_recv_buf;
    buffer* unpack_buf;

    std::queue<st_io*> send_list;
    process_packet _process_packet;
    on_disconnect _on_disconnect;
    bool mt;
    bool raw;
    bool is_close;
    uint32_t last_recv;
    uint32_t recv_timeout;
    uint32_t send_timeout;
    
        
};
}

#endif
