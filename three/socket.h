//
// (nwlzee AT gmail DOT com)
//

#ifndef __HULUZY_SOCKET_H__
#define __HULUZY_SOCKET_H__

namespace huluzy {

class Socket {
 public:
    Socket();
    ~Socket();

    ssize_t ReadFromSocket(int sockfd,const struct iovec* iov, int iovcnt);
    ssize_t WriteToSocket(int sockfd,const struct iovec* iov, int iovcnt);
};

}

#endif
