//
// Author: zhanglao (nwlzee at gmail dot com)

#ifndef __HULUZY_COMMON_THREAD_H__
#define __HULUZY_COMMON_THREAD_H__

#include "queue.h"
#include "common/noncopyable.h"
#include "job.h"

namespace huluzy {
class Thread : noncopyable {
  public:
    Thread(DLinkedListQueue<Job> *tqueue = NULL);
    ~Thread();
    void Start();
    void Stop();
  private:
    void ThreadFunc();
    //    int FetchJobFromThreadPool();
    static void*  RunInThread(void*);
    //    JobList *jobl_;
    pthread_t ptid_;
    bool qshared_;
//    Mutex lock_;
    DLinkedListQueue<Job> *tqueue_;
    bool start_;
    friend class ThreadPool;
};

}  // namespace huluzy


#endif
