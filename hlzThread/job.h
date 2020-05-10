//    
// Author : zhanglao ( nwlzee at gmail dot com)
//   

#ifndef __HULUZY_COMMON_JOB_H__
#define __HULUZY_COMMON_JOB_H__

namespace huluzy {
typedef int (*JobProc)(void*, void*);

struct Job 
{

  Job *next,*prev;
  void* jobContext;
  JobProc fproc;
};

}

#endif 
