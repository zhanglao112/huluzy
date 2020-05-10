//     
// Author: zhanglao ( nwlzee at gmail dot com)
// 

#ifndef  __HULUZY_IOEVENTLOOP_H__
#define  __HULUZY_IOEVENTLOOP_H__

#include <vector>
#include "handler.h"


namespace huluzy {

class Epoller;
//class Handler;

class IOEventLoop {
 public:
  IOEventLoop();
  ~IOEventLoop();
  void start();
  void stop();
  void updateIOHandler(Handler*);
  void removeIOHandler(Handler*);
 private:
  std::vector<IOHandler*> _activers;
  IOHandler* _current;
  Epoller* _poll;
  bool start_;
  IOEventLoop(const IOEventLoop& );
  const IOEventLoop &operator = (const IOEventLoop&);
  
};
} // namespace huluzy

#endif
