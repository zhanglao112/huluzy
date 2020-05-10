#include "acceptor.h"

using namespace huluzy;

Accptor::Accptor(IOEventloop* loop) :
    IOHandler(loop) {
    
}

Acceptor::~Acceptor() {

}

void Acceptor::Init(char *ipaddr, int port) {
    struct sockaddr_in s_addr;
    bzero(&s_addr, sizeof s_addr);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port  = htons(port);
    inet_pton(AF_INET,ipaddr,&(s_addr.sin_addr));

    _sockfd = socket(AF_INET,SOCK_STREAM,0);
    ::bind(_sockfd,(struct sockaddr*)&s_addr,sizeof s_addr );
}

void Acceptor::Listen(int backlog) {
    ::listen(_sockfd,backlog);
    want_read();
}

void Accpetor::HandleRead() {
    while(1) {
        HANDLE nfd = ::accept(_sockfd,NULL,NULL);
        if(-1 == nfd) {
            if ((EAGAIN == errno) || (EWOULDBLOCK == errno)) {
                // we would have connect all income client
                break;
            } else {
                FATAL("Acceptor::accept");
            }
        }
        IOHandler *new_conn = new Connection(nfd,m_loop);
        new_conn->want_read();
        new_conn->want_write();
    }
}
