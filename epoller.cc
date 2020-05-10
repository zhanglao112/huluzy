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


Epoller::Epoller():epollfd_(epoll_create1(0)),
					events_(kInitEventListSize)
{

}


Epoller::~Epoller() {
  close(epollfd_);
}

void Epoller::poll(int timeout,std::vector<Handler*> *phandlerList) {
//	assert (events_.size() > 0)std::cout<<events_.size()<<std::endl;
  int nEvents = ::epoll_wait(epollfd_,
		       &*events_.begin(),10,
		       //static_cast<int>(events_.size()),
		       -1);
  if (nEvents > 0) {
    fillActiveHandlers(nEvents,phandlerList);
    if (nEvents == static_cast<int>(events_.size())) {
      events_.resize(events_.size() * 2);
    }
  } else if (nEvents == 0) {
    std::cout << "nothing happen!"<<std::endl;
  } else {
    perror("Epoller::Poll");
  }

}

void Epoller::fillActiveHandlers(int nEvents,HandlerList *phandlerList) const  {
  for (int i = 0; i< nEvents;++i) {
    Handler *handler = static_cast<Handler*>(events_[i].data.ptr);
    handler->set_revents(events_[i].events);
    phandlerList->push_back(handler);
  }
}

void Epoller::UpdateHandler(Handler* phandler) {
  int fd = phandler->get_fd();
  //HandlerMap::const_iterator map_it = handlers_.find(fd);
  HandlerMap::const_iterator  map_it = handlers_.find(fd);
  //handlers_.count(fd); // 0 or 1
  if( map_it != handlers_.end()) {
    update(EPOLL_CTL_ADD,phandler);
  } else {
    update(EPOLL_CTL_MOD,phandler);
  }
}

void Epoller::update(int operation,Handler* phandler) {
  struct epoll_event event;
  bzero(&event,sizeof event);
  int fd = phandler->get_fd();
  event.data.fd = fd;
  event.events = phandler->get_events();
  event.data.ptr = phandler;

  if(::epoll_ctl(epollfd_,operation,fd,&event) < 0) {
    perror("epoll_ctl");
  }
  
}

void Epoller::RemoveHandler(Handler* phandler) {
  if ( NULL == phandler) {
    return;
  }
  update(EPOLL_CTL_DEL,phandler);
  handlers_.erase(phandler->get_fd());
  // TODO: delete phandler
}
