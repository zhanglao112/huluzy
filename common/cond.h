//
// Author: zhanglao (nwlzee at gmail dot com)
//

#ifndef __HULUZY_COND_H__
#define __HULUZY_COND_H__

//#include <time.h>
namespace huluzy {

class Mutex;

class Cond
{
 public:
  explicit  Cond(Mutex& mutex);
  ~Cond();
  void wait();
  bool timedWait(struct timespec* timeout);
  void signal();
  void broadcast();
 private:
  Mutex& mlock;
  pthread_cond_t cond_;
};
}

#endif 
