#include "socket.h"

namespace huluzy {
Socket::Socket() {

}

Socket::~Socket() {

}

ssize_t Socket::ReadFromSocket(int sockfd, const struct iovec *iov, int iocnt) {
    ssize_t res;
    do {
        res = ::readv(sockfd, iov, iocnt);
    } while(-1 == res && errno == EINTR);

    return res;
}

ssize_t Socket::WriteToSocket(int sockfd, const struct iovec *iov, int iocnt) {
    ssize_t res;
    do {
        res = ::writev(sockfd, iov, iocnt);
    } while (-1 == res && errno == EINTR);
    
    return res;
    ::writev(sockfd, iov, iocnt);
}

}
