//
// Author: zhanglao (nwlzee at gmail dot com)
//

#ifndef __HULUZY_COMMON_JOBLIST__
#define __HULUZY_COMMON_JOBLIST__

namespace huluzy {


class JobList : private noncopyable {
  public:
    //    typedef T1* (*jobFunc)(T2*) JobFunc;
    JobList();
    ~JobList();
    void InsertRange(void *start,void *end);
    void InsertOne(void *start);
    void RemoveRange(void *start,void *end) {}
    void* RemoveOne();
    int GetJobNum(){ return kjobnum_;}
  private:
    struct JobEntry {
      void *head;
      void *tail;
    };
    int kjobnum_;
    JobEntry job_entry_; 
    
};
}

#endif
