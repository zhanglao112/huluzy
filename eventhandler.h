// 
// ( nwlzee AT gmail DOT com)
//

#ifndef __HULUZY_EVENTHANDLER_H__
#define __HULUZY_EVENTHANDLER_H__

namespace huluzy {

class EventHandler {
 public:
    EventHandler() {}
    virtual ~EventHandler() {}
    virtual HandleEvent() = 0;
 private:
    EventHandler(const EventHandler&);
    const EventHandler &operator = (const EventHandler&);
};
}

#endif
