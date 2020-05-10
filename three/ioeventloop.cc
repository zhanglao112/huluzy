#include <iostream>
#include <algorithm>
//#include "epoller.h"
//#include "handler.h"
#include "ioeventloop.h"
#include "epoller.h"

using namespace huluzy;

IOEventLoop::IOEventLoop():_current(NULL),
				start_(false)
{
  poll_ = new Epoller();
}

IOEventLoop::~IOEventLoop(){
  delete poll_;
}

void IOEventLoop::start() {
  start_ = true;
	while(start_) {
  poll_->poll(1,&_activers);
  for (std::vector<IOHandler*>::iterator it=_activers.begin();
    it !=_activers.end();++it) {
    _current= (Handler*)*it;
    if (_current->AEvents() & EPOLLIN)
        _current->HandleRead();
    if (_current->AEvents() & EPOLLOUT)
        _current->HandleWrite();
    if (_current->AEvents() & EPOLLERR)
        _current->HandleRead();
        //TODO : delete
  }
  _current = NULL;
}
  
}

void IOEventLoop::stop(){
  start_  = false;
}

void IOEventLoop::updateHandler(IOHandler* iohandler) {
  if (NULL == iohandler) {
    return;
  }
  poll_->UpdateHandler(iohandler);
}

void IOEventLoop::removeHandler(IOHandler* iohandler) {
  if (NULL == iohandler) {
    return;
  }
  if (current_ != iohandler ||
      *std::find(_activers.begin(),_activers.end(),iohandler) != iohandler) {
      poll_->RemoveHandler(phandler);
  }
}
