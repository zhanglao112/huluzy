//
// (nwlzee AT gmail DOT com)
//

#ifndef __HULUZY_IOHANDLER_H__
#define __HULUZY_IOHANDLER_H__

#include "defines.h"

namespace huluzy {
class IOHandler {
 public:
    IOHandler(IOEventLoop* loop) {
        m_loop = loop;
    }
    virtual ~IOHandler() {
        ::close(_sockfd);
    }
    virtual void HandleRead() = 0;
    virtual void HandleWrite() = 0;
    void want_read() {
        //m_loop->registerEvent(_hfd,EV_READ);
        _events |= EV_READ;
        m_loop->updateHandler(this);

    }
    virtual  void want_write() {
        // m_loop->registerEvent(_hfd,EV_WRITE);
        _events |= EV_WRITE;
        m_loop->updateHandler(this);
    }



    void set_events(int evts) {
        _events = evts;
    }

    int get_events() const {
        return _events;
    }

    void on_active(int evts) {
        _aevents = evts;
    }
    int AEvents() const {
        return _aevents;
    }
    HANDLE get_fd() const {
        return _sockfd;
    }
 protected:
    HANDLE _sockfd;
    IOEventLoop *m_loop;
    int _events;
    int _aevents;
 private:
    IOHandler(const IOHandler&);
    const IOHandler &operator = (const IOHandler&);

    
};
}
#endif
