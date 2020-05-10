#include <pthread.h>
#include <cassert>
#include <cstdio>
#include <errno.h>
#include "cond.h"
#include "mutex.h"

namespace huluzy {
  
Cond::Cond(Mutex& mutex)
  : mlock(mutex)
{
  int ret = pthread_cond_init(&cond_,NULL);
  assert(ret == 0); (void) ret;
}

Cond::~Cond() 
{
  int ret = pthread_cond_destroy(&cond_);
  assert(ret == 0); (void) ret;
}

void Cond::wait() {
  int ret = pthread_cond_wait(&cond_,&mlock.mutex_);
  assert(ret == 0); (void) ret;
}

bool Cond::timedWait(struct timespec* ts) {
  int ret = pthread_cond_timedwait(&cond_,&mlock.mutex_,ts);
  if (ret  < 0 && ret != ETIMEDOUT) {
    fprintf(stderr,"pthread_cond_timewait error");
  }
  if (ret == ETIMEDOUT) {
    return false;
  }
  return true;
}

void Cond::signal() {
  int ret = pthread_cond_signal(&cond_);
   assert(ret == 0); (void) ret;
}

void Cond::broadcast() {
  int ret = pthread_cond_broadcast(&cond_);
  assert(ret == 0); (void) ret;
}

}
