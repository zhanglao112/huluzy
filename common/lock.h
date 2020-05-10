// 
// Author: zhanglao ( nwlzee at gmail dot com)
//

#ifndef __HULUZY_LOCK_H__
#define __HULUZY_LOCK_H__

namespace huluzy {
  
template <typename T>
class LockGuardT
{
 public:
  explicit LockGuardT(const T& mutex) :
      mutex_(mutex)
  {
    mutex_.lock();
    acquired_ = true;
  }
  
      ~LockGuardT() {
	if (acquired_) {
	  mutex_.unlock;
	}
      }
      bool acquired() const {
	return acquired_;
      }
      
 private:
      LockGuardT(const LockGuardT&);
      LockGuardT& operator = (const LockGuardT&);

      const T& mutex_;
      bool acquired_;
 };
}

#endif 
