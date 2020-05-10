#include "mutex.h"
#include "cond.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <unistd.h>

using namespace huluzy;

Mutex m;
Cond c(m);

void* routine1(void*) {
  printf("I'M Waiting...--routine1\n");
  c.wait();
  printf("OK! I have received the signal--routine1\n");
}
void* routine2(void*) {
  printf("I'm sending the signal...--routine2\n");
  c.signal();
	sleep(2);
  printf("OK! I have sended the signal. ---routine2\n");
}

int main() {
  int pthreadCount = 2;
  pthread_t pths[pthreadCount];
  int ret = pthread_create(&pths[0],NULL,routine1,NULL);
  if (ret != 0) {
    assert("pthread_create 1");
  }
  sleep(2);
  ret = pthread_create(&pths[1],NULL,routine2,NULL);
	if (ret != 0) {
		assert("pthread_create 2");
	}
  
  for ( int i = 0; i< pthreadCount; i++) {
    pthread_join(pths[i],NULL);
  }

  return 0;
  
}
