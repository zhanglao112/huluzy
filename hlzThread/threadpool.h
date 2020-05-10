//
// Author: zhanglao (nwlzee at gmail dot com)
//

#ifndef __HULUZY_COMMON_THREADPOOL_H__
#define __HULUZY_COMMON_THREADPOOL_H__

#include "thread.h"
#include <vector>
#include "common/noncopyable.h"

namespace huluzy {
class ThreadPool : noncopyable {
  public:
    ThreadPool();
    ~ThreadPool();
    void Start(int workthreads);
    void Stop();
    void QueueJob(Job* pJob,int nWhich);
    bool CancelJob(Job* pJob,int nWhich);
  private:
    int nWorkers_;
    int nJobs_;
    std::vector<Thread*> workThreads_;
    //    bool running_;
  };
}  // namespace huluzy

#endif
