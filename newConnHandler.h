//
// Author: zhanglao ( nwlzee at gmail dot com)
//

#ifndef __HULUZY_NEWCONNHANDLER_H__
#define __HULUZY_NEWCONNHANDLER_H__

namespace huluzy {


class NewConnHandler : public Handler {
 public:
  NewConnHandler(int,IOEventLoop*);
  ~NewConnHandler();
  void OnEvent();
};
}  // namespace huluzy

#endif
