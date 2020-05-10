#include "engine.h"

using namespace huluzy;

Engine::Engine() {
    poller_fd = epoll_create(MAX_SOCKET);
}

Engine::~Engine() {
    ::close(poller_fd);
}

int32_t Engine::epoll_register(Socket* s) {
    assert(s);
    int32_t ret = -1;
    struct epoll_event ev;
    ev.data.ptr = s;
    ev.events = EV_IN | EV_OUT | EV_ET;
    ret = ::epoll_ctl(poller_fd,EPOLL_CTL_ADD,s->fd_,&ev);
    
    return ret;
}

inline int32_t Engine::epoll_unregister(Socket *s) {
    assert(s);
    struct epoll_event ev;
    int32_t ret;
    ret = ::epoll_ctl(poller_fd,EPOLL_CTL_DEL,s->fd_,&ev);
    s->readable_ = s->writeable_ = 0;
    // ???
    //double_link_remove((struct double_link_node*)s);
    s->engine = NULL;
    return ret;
}

int32_t Engine::epoll_loop(int32_t ms) {
    uint32_t sleep_ms;
    uint32_t timeout = GetSystemMs() + ms;
    uint32_t current_tick;
    do {
        while(!actived.empty()) {
            Socket *s = (Socket*)actived.pop();
            s->isactived = 0;
            if(Socket::Process(s)) {
                s->isactived = 1;
                actived.push_back(s);
            }
        }
        current_tick = GetSystemMs();
        sleep_ms = timeout > current_tick ? timeout - current_tick : 0;
        int32_t nfds = ::epoll_wait(poller_fd,events,MAX_SOCKET,sleep_ms);
        if(nfds < 0) 
            return -1;
        for (int32_t i = 0; i< nfds; ++i) {
            Socket *sock = (Socket*)events[i].data.ptr;
            if(sock) {
                if(events[i].events & EPOLLIN)
                    sock->on_read_active();
                if(events[i].events & EPOLLOUT)
                    sock->on_write_active();
                if(events[i].events & EPOLLERR)
                    sock->on_read_active();
            }
        }
        current_tick = GetSystemMs();
    }while(timeout > current_tick);
    return 0;
}
