//
// zhanglao (nwlzee at gmail dot com)
//
#include <stdlib.h>
#include <pthread.h>
#include <cstring>
#include "threadpool.h"
#include "common/hlzAtomic.h"

using namespace huluzy;


ThreadPool::ThreadPool():nWorkers_(0),
			 nJobs_(0)
			 //			 running_(false)
{

}

ThreadPool::~ThreadPool() {

}

void ThreadPool::Start(int workThreads) {
  nWorkers_ = workThreads;
  workThreads_.reserve(workThreads);
  for (int i = 0; i< workThreads;++i) {
    Thread *new_thread = new Thread;
    workThreads_.push_back(new_thread);
    nWorkers_++;
    new_thread->Start();
  }
}

void ThreadPool::Stop() {
  for (int i= 0;i< workThreads_.size();i++) {
    workThreads_[i]->Stop();
	delete workThreads_[i];
    nWorkers_--;
  }
}

void ThreadPool::QueueJob(Job* pJob,int nWhich) {
  int nJobs = atomic_add(&nJobs_,1);
  if ( -1 == nWhich) {
    nWhich = nJobs_ % nWorkers_;
  }
  workThreads_[nWhich]->tqueue_->Push(pJob);
}

static int  DoNothing(void *pJob,void* p2) {
  free((Job*)pJob);
	return 0;
}

bool ThreadPool::CancelJob(Job* pJob,int nWhich) {
  Job*p = (Job*)malloc(sizeof(Job));
  memset(p,0,sizeof(Job));
  p->fproc = DoNothing;
  
  if( workThreads_[nWhich]->tqueue_->Replace(pJob,p) ) {
    //    free(pJob);
    return true;
  }
  free(p);
  return false;
}
