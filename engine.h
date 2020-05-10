#ifndef __HULUZY_ENGINE_H__
#define __HULUZY_ENGINE_H__

namespace huluzy{

#deifne EV_IN   EPOLLIN
#define EV_OUT  EPOLLOUT
#define EV_ERR  EPOLLERR
#define EV_ET   EPOLLET

class Socket;

class Engine {
    Engine();
    ~Engine();
    int32_t registerToLoop(Socket*);
    int32_t unregister(Socket*);
    volatile int32_t status;
    int32_t poller_fd;
    struct epoll_event events[MAX_SOCKET];
    std::queue<st_io*> *actived;
};

}

#endif
