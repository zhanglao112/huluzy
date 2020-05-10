//
// (nwlzee AT gmail DOT com)
//

#ifndef __HULUZY_CONNECTION_H__
#define __HULUZY_CONNECTION_H__

#define MAX_WBUF_COUNT 512
#define MAX_SEND_SIZE 4096

namespace huluzy {

class Packet;
class Connection : public IOHandler{
 public:
    Connection(int sockfd,IOEventloop* loop);
    ~Connection();
    void HandleRead();
    void HandleWrite();
 private:
    void make_packet();
    uint32_t prepare_send();
 private:
    struct iovec wiov_[MAX_WBUF_COUNT];
    struct iovec rbuf[2];
    Buffer* m_rbuffer;
    Buffer* m_wbuffer;
    Buffer* m_unpackbuf;
    uint32_t rpos_;
    uint32_t wpos_;
    uint32_t unpackpos_;
    uint32_t unpacksize_;
    bool raw_;
    std::queue<Packet*> wq_;
    std::queue<Packet*> rq_;

    Socket m_socket;
    PacketProcess *m_packetProcess;
 private:
    Connection(const Connection&);
    const Connection &operator = (const Connection&);
};

}

#endif
