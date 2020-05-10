//
// Author: zhanglao (nwlzee at gmail dot com)
//

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <new>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include "acceptor.h"
// #include "handler.h"

#include "ioeventloop.h"


using namespace huluzy;

class TcpServerHandler : public Handler {
  public:
	TcpServerHandler(int fd,IOEventLoop* loop) : Handler(fd, loop) {
		rbuf = (char*)malloc(1024);
		wbuf = (char*)malloc(1024);
		rpos = wpos = 0;
  
		// do it in acceptor
		//  set_events(EPOLLIN|EPOLLOUT);
		//registerToLoop();
	}

	~TcpServerHandler() {
		free(rbuf);
		free(wbuf);
	}

	void handle_write(char*buf,int buflen) {
		memcpy(wbuf,wbuf+wpos,buflen);
		wpos += buflen;
	}

	void handle_read(char *rstr) {
		memcpy(rstr,rbuf,rpos+1);
		rpos = 0;
	}

	void OnEvent() {
		int revents = get_revents();
		int once_flag = 0;
		if (revents & EPOLLIN) {
			while( !once_flag) {
				int rsize = ::recv(get_fd(),rbuf+rpos,1024,0);
      
				if(rsize < 0) {

					// have no more data
					if (errno == EAGAIN) {
						break;
					} else {
						return;
					}
				} else if (0 == rsize){
					// peer socket have closed
					close(get_fd());
				} 
				if (rsize == 1024) {
					once_flag = 1;
				} else {
					once_flag = 0;
				}
      
				rpos += rsize;
				std::cout<<rbuf<<std::endl;
			}
		} else if (revents & EPOLLOUT) {
			char str[] = "hello";
			handle_write(str,5);
			::send(get_fd(),wbuf,wpos,0);
			wpos = 0;

		} else {
			shutdown(get_fd(),SHUT_WR);
		}
	}
  private:
  	char *rbuf;
  	char *wbuf;
  	int rpos;
  	int wpos;
};

template <typename T>
class TcpServer {
  public:
	TcpServer(char* ipaddr,int port) {
		m_loop = new IOEventLoop();
		m_acceptor = new Acceptor<T>(-1,m_loop);
		//  m_tpcHandler = new T(-1,m_loop);
		ipaddr_ = (char*)malloc(20);
		strncpy(ipaddr_,ipaddr,20);
		port_ = port;
	}

	~TcpServer() {
		m_acceptor->close();
		m_loop->stop();

		delete m_acceptor;
		delete m_loop;
		free(ipaddr_);
	}

	void start() {
		m_loop->start();
		m_acceptor->open(ipaddr_,port_);
	}

	void stop() { }
  
  private:
		Acceptor<T> *m_acceptor;
		// T *m_tcpHandler;
		IOEventLoop *m_loop;
		char* ipaddr_;
		int port_;
};

