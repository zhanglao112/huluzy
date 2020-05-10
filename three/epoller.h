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
	 virtual void poll(int timeout,std::vector<IOHandler*> *ioHList);
	void UpdateHandler(IOHandler*);
	void RemoveHandler(IOHandler*);
  
  private:
	typedef std::vector<struct epoll_event> EventList;
	typedef std::map<int,IOHandler*> IOHandlerMap;
	typedef std::vector<IOHandler*> HandlerList;
  
	void update(int operation,IOHandler* phandler);
	void fillActiveIOHandlers(int,HandlerList*) const;
	//struct epoll_event event_;
	static const int kInitEventListSize = 16;
	int _epollfd;
	bool _start;
	EventList _events;
	IOHandlerMap _ioHandlers;

	Epoller(const Epoller&);
	const Epoller &operator = (const Epoller&);

};
}


#endif
