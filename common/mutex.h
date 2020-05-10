// Author: zhanglao (nwlzee at gmail dot com)
//

#ifndef HULUZY_BASE_MUTEX_H
#define HULUZY_BASE_MUTEX_H

#include "noncopyable.h"
#include <pthread.h>
#include <cassert>

namespace huluzy
{
class Mutex : noncopyable {
 private:
   mutable pthread_mutex_t mutex_;
  friend class Cond;
 public:
  Mutex() {
    const int ret = pthread_mutex_init(&mutex_,NULL);
	assert(ret == 0); (void)ret;
  }
  ~Mutex() {
    const int ret = pthread_mutex_destroy(&mutex_);
	assert(ret == 0); (void)ret;
  }
  void Lock() const{
    const int ret = pthread_mutex_lock(&mutex_);
	assert(ret == 0); (void)ret;
  }
  
  void Unlock() const {
    const int ret = pthread_mutex_unlock(&mutex_);
	assert(ret == 0); (void)ret;
  }
  
  bool tryLock() const {
	const int ret = pthread_mutex_trylock(&mutex_);
	// TODO : ret == EBUSY , EDEADLK log
	return ret == 0;   
  }
  
};

class MutexHolder : noncopyable{
 private:
  Mutex& mutex_;
 public:
  explicit MutexHolder(Mutex& mutex) : mutex_(mutex) {
    mutex_.Lock();
  }

  ~MutexHolder() {
    mutex_.Unlock();
  }
};
}  // namespace huluzy

#endif 
