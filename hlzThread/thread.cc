#include "thread.h"

using namespace huluzy;

Thread::Thread(DLinkedListQueue<Job>* tqueue):start_(false),
					       ptid_(0)
{
  if (tqueue) {
    tqueue_ = tqueue;
    qshared_ = true;
  } else {
    tqueue_ = new DLinkedListQueue<Job>();
    qshared_ = false;
  }
}

Thread::~Thread() {
  if (!qshared_) {
    delete tqueue_;
  }
}

void Thread::ThreadFunc() {
  while (start_) {
      Job *job;
      while (job = (Job*)tqueue_->Pop()) {
	if (job->fproc) {
	  job->fproc(job,job->jobContext);
	}
      }
    }
}

void* Thread::RunInThread(void* Arg) {
  Thread *pth = (Thread*)(Arg);
  pth->ThreadFunc();
}

void Thread::Start() {
  start_ = true;
  int ret = pthread_create(&ptid_,NULL,RunInThread,this);
}


void Thread::Stop() {
  start_ = false;
  pthread_join(ptid_,NULL);
}
