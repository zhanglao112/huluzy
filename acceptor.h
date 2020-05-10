//
//Author: zhanglao ( nwlzee at gmail dot com)
//

#ifndef __HULUZY_ACCEPTOR_H__
#define __HULUZY_ACCEPTOR_H__

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <sys/un.h>
#include "handler.h"
//class IOEventLoop;

namespace huluzy {

template<typename T>
class Acceptor : public Handler {
  public:
 	Acceptor(int fd, IOEventLoop* loop):Handler(fd, loop) { }
	~Acceptor() { }
	void open(char* ipaddr,int port) {
		struct sockaddr_in s_addr;
		int qlen = 5;
		bzero(&s_addr,sizeof s_addr);
		s_addr.sin_family = AF_INET;
		s_addr.sin_port = htons(port);
		inet_pton(AF_INET,ipaddr,&(s_addr.sin_addr));

		int fd = socket(AF_INET,SOCK_STREAM,0);
		set_fd(fd);
		::bind(get_fd(),(struct sockaddr*)&s_addr,sizeof s_addr);
		::listen(get_fd(),qlen);
		set_events(EPOLLIN);
		registerToLoop();
	}
	void close() { }

	void OnEvent() {
		int revents = get_revents();
		int fd = get_fd();
  
		if (revents & EPOLLIN) {
			while(1) {
				int c_fd = ::accept(fd,NULL,NULL);
				if ( -1 == c_fd) {
					if (( errno == EAGAIN) || (errno == EWOULDBLOCK) ) {
						// we would have connect all income client
						break;
					} else {
	    				perror("Acceptor::accept");
	  				}
				}
			
				Handler* pHandler = new T(c_fd,loop_);
				pHandler->set_events(EPOLLIN|EPOLLOUT);
				pHandler->registerToLoop();
			}
		}
	}
  private:
	Acceptor(const Acceptor&);
	const Acceptor &operator = (const Acceptor&);
  
};

}  // namespace huluzy

#endif
