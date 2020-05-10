//
// zhanglao (nwlzee @ gmail dot com)
//

#ifndef __HULUZY_COMMON_JOBLIST__
#define __HULUZY_COMMON_JOBLIST__

namespace huluzy {
  struct JobProc {
    jobproc *next;
    void *proc;
  };
}  // namespace huluzy

#endif
