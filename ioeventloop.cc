#include <iostream>
#include <algorithm>
//#include "epoller.h"
//#include "handler.h"
#include "ioeventloop.h"
#include "epoller.h"

using namespace huluzy;

IOEventLoop::IOEventLoop():currentActiveHandler_(NULL),
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
  poll_->poll(1,&activeHandlers_);
  for (std::vector<Handler*>::iterator it=activeHandlers_.begin();
    it !=activeHandlers_.end();++it) {
    currentActiveHandler_= (Handler*)*it;
    currentActiveHandler_->OnEvent();
    //TODO : delete
  }
}
  
}

void IOEventLoop::stop(){
  start_  = false;
}

void IOEventLoop::updateHandler(Handler* phandler) {
  if (NULL == phandler) {
    return;
  }
  poll_->UpdateHandler(phandler);
}

void IOEventLoop::removeHandler(Handler* phandler) {
  if (NULL == phandler) {
    return;
  }
  if (currentActiveHandler_ != phandler ||
      *std::find(activeHandlers_.begin(),activeHandlers_.end(),phandler) != phandler) {
      poll_->RemoveHandler(phandler);
  }
}
