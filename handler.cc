#include "ioeventloop.h"
#include "handler.h"

using namespace huluzy;

Handler::Handler(int fd,IOEventLoop* loop)
  :hfd_(fd),
	loop_(loop)
{
  
}

Handler::~Handler()
{
}

void Handler::registerToLoop(){
  loop_->updateHandler(this);
}

void Handler::removeFromLoop(){
  loop_->removeHandler(this);
}
