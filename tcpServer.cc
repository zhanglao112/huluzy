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

#include "tcpServer.h"

using namespace huluzy;

TcpServerHandler::TcpServerHandler(int fd,IOEventLoop* loop)
  : Handler(fd,loop)
{
  rbuf = (char*)malloc(1024);
  wbuf = (char*)malloc(1024);
  rpos = wpos = 0;
  
  // do it in acceptor
  //  set_events(EPOLLIN|EPOLLOUT);
  //registerToLoop();
}

TcpServerHandler::~TcpServerHandler() {
  free(rbuf);
  free(wbuf);
}

void TcpServerHandler:: OnEvent() {
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
    handle_write("hello",5);
    ::send(get_fd(),wbuf,wpos,0);
    wpos = 0;
    
  } else {
    shutdown(get_fd(),SHUT_WR);
  }
}

void TcpServerHandler::handle_write(char* buf,int buflen) {
  memncpy(wbuf,wbuf+wpos,buflen);
  wpos += buflen;
}

void TcpServerHandler::handle_read(char*rstr) {
  memncpy(rstr,rbuf,rpos+1);
  rpos = 0;
}

<template typename T>
TcpServer<T>::TcpServer(char* ipaddr,int port) {
  m_loop = new IOEventLoop();
  m_acceptor = new Acceptor<T>(-1,m_loop);
  //  m_tpcHandler = new T(-1,m_loop);
  ipaddr_ = ipaddr;
  port_ = port;
}

<template typename T>
TcpServer<T>::~TcpServer() {
  m_acceptor.close();
  m_loop.stop();
  delete m_acceptor;
  delete m_loop;
}

<template typename T>
TcpServer::start() {
  m_loop.start();
  m_acceptor.open(ipaddr_,port_);
}

<template typename T>
TcpServer::stop() {
  
}


int main() {
  TcpServer<TcpServerHandler> ts("127.0.0.1",4999);
  ts.start();
}
