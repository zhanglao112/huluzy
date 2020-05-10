#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>   // sleep
#include "job.h"
#include "threadpool.h"

using namespace huluzy;

static __thread int thdata;

static int  testFunc(void* p1,void* p2) {
//	sleep(1);
	thdata++;
  printf("in threadid = %d,the data is %d\n",pthread_self(),thdata);
	return 0;
}

int main() {
  int maxjob = 256;
  int startthreads = 4;
  ThreadPool pool;
  pool.Start(startthreads);
  Job job[maxjob];
  for (int i = 0;i < maxjob;++i) {
    job[i].jobContext = NULL;
    job[i].fproc = testFunc;
    pool.QueueJob(&job[i],i% startthreads);
  }
  //sleep(60);
  pool.Stop();
}
