//
// Author: zhanglao ( nwlzee at gmail dot com)
// 

#ifndef __HULUZY_HANDLER_H__
#define __HULUZY_HANDLER_H__

#include <cstdlib>

namespace huluzy {

class IOEventLoop;

class Handler {
 public:
  Handler(int,IOEventLoop*);
 virtual  ~Handler();
 virtual void handle_read() {} 
 virtual void handle_write() {}
 virtual void OnEvent() = 0;


  void registerToLoop();
  void removeFromLoop();

  void set_loop(IOEventLoop* loop) {
    if ( NULL == loop_) {
      loop_ = loop;
    }
  }

  void set_fd(int fd) {
    // TODO: hfd_ is set once.
    hfd_ = fd;
  }
  int get_fd() const {
    return hfd_;
  }
  void set_events(int evts) {
    events_ = evts;
  }
  int get_events() const {
    return events_;
  }
  void set_revents(int revts) {
    revents_ = revts;
  }
  int get_revents() const {
    return revents_;
  }

 private:
  int hfd_;
  int events_;
  int revents_;
	protected:
		IOEventLoop* loop_;

  Handler(const Handler&);
  const Handler &operator = (const Handler&);
  
};

} // namespace huluzy 

#endif
