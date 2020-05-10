#include "newConnHandler.h"

using namespace huluzy;

NewConnHandler::ConnHandler(int fd,IOEventLoop* loop)
  : Handler(fd,loop) {

}

~NewConnHandler() {

}

void OnEvent() {
  int revents = get_revents();
  int once_flag = 0;
  if (revents & EPOLLIN) {
    while( !once_flag) {
      int r_size = ::recv(get_fd(),pbuf_,buflen_,0);
      if(r_size < 0) {

	// have no more data
	if (errno == EAGAIN) {
	  break;
	} else {
	  return;
	}
      } else if (0 == r_size){
	// peer socket have closed
	close(get_get());
      } 
      if (r_size == buflen) {
	once_flag = 1;
      } else {
	once_flag = 0;
      }
    }
  } else if (revents & EPOLLOUT) {
    ::send(get_fd(),pbuf_,buflen_,0);
  } else {
    shutdown(get_fd());
  }
}
