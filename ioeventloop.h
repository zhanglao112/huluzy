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
  void updateHandler(Handler*);
  void removeHandler(Handler*);
 private:
  std::vector<Handler*> activeHandlers_;
  Handler* currentActiveHandler_;
  Epoller* poll_;
  bool start_;
  IOEventLoop(const IOEventLoop& );
  const IOEventLoop &operator = (const IOEventLoop&);
  
};
} // namespace huluzy

#endif
