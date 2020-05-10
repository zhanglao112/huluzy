// 
// Author: zhanglao ( nwlzee at gmail dot com) 
//

#include <sys/epoll.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include "epoller.h"
//#include "handler.h"

using namespace huluzy;


Epoller::Epoller():_epollfd(epoll_create1(0)),
					_events(kInitEventListSize)
{

}


Epoller::~Epoller() {
    ::close(_epollfd);
}

void Epoller::poll(int timeout,std::vector<IOHandler*> *ioHList) {
//	assert (events_.size() > 0)std::cout<<events_.size()<<std::endl;
  int nEvents = ::epoll_wait(epollfd_,
		       &*events_.begin(),10,
		       //static_cast<int>(events_.size()),
		       -1);
  if (nEvents > 0) {
    fillActiveHandlers(nEvents,ioHList);
    if (nEvents == static_cast<int>(events_.size())) {
      events_.resize(events_.size() * 2);
    }
  } else if (nEvents == 0) {
    std::cout << "nothing happen!"<<std::endl;
  } else {
    perror("Epoller::Poll");
  }

}

void Epoller::fillActiveHandlers(int nEvents,HandlerList *ioHList) const  {
  for (int i = 0; i< nEvents;++i) {
    IOHandler *handler = static_cast<IOHandler*>(events_[i].data.ptr);
    handler->on_active(events_[i].events);
    ioHList->push_back(handler);
  }
}

void Epoller::UpdateHandler(IOHandler* iohandler) {
  int fd = iohandler->get_fd();
  //HandlerMap::const_iterator map_it = handlers_.find(fd);
  HandlerMap::const_iterator  map_it = _ioHandlers.find(fd);
  //handlers_.count(fd); // 0 or 1
  if( map_it != _ioHandlers_.end()) {
    update(EPOLL_CTL_ADD,iohandler);
  } else {
    update(EPOLL_CTL_MOD,iohandler);
  }
}

void Epoller::update(int operation,IOHandler* iohandler) {
  struct epoll_event event;
  bzero(&event,sizeof event);
  int fd = iohandler->get_fd();
  event.data.fd = fd;
  event.events = iohandler->get_events();
  event.data.ptr = iohandler;

  if(::epoll_ctl(epollfd_,operation,fd,&event) < 0) {
    perror("epoll_ctl");
  }
  
}

void Epoller::RemoveHandler(IOHandler* iohandler) {
  if ( NULL == iohandler) {
    return;
  }
  update(EPOLL_CTL_DEL,iohandler);
  handlers_.erase(iohandler->get_fd());
  // TODO: delete phandler
}
