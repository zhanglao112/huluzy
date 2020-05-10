//    
// Author: zhanglao ( nwlzee at gmail dot com)
//

#ifndef __HULUZY_EPOLLER_H__
#define __HULUZY_EPOLLER_H__

//#include <unistd.h>
#include <vector>
#include <map>
#include "handler.h"

namespace huluzy {

//class Handler;

class Epoller {
  public:
	Epoller();
	virtual ~Epoller();
	 virtual void poll(int timeout,std::vector<Handler*> *phandlerList);
	void UpdateHandler(Handler*);
	void RemoveHandler(Handler*);
  
  private:
	typedef std::vector<struct epoll_event> EventList;
	typedef std::map<int,Handler*> HandlerMap;
	typedef std::vector<Handler*> HandlerList;
  
	void update(int operation,Handler* phandler);
	void fillActiveHandlers(int,HandlerList*) const;
	//struct epoll_event event_;
	static const int kInitEventListSize = 16;
	int epollfd_;
	bool start_;
	EventList events_;
	HandlerMap handlers_;

	Epoller(const Epoller&);
	const Epoller &operator = (const Epoller&);

};
}


#endif
