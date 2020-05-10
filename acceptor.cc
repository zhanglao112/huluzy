
#include "acceptor.h"

using namespace huluzy;

<template typename T>
Acceptor::Acceptor(int fd,IOEventLoop* loop):Handler(fd,loop)
{
  
}

<template typename T>
Acceptor::~Acceptor()
{

}

<template typename T>
Acceptor::open(char* ipaddr,int port) {
  struct sockaddr_in s_addr;
  int qlen = 5;
  bzero(&s_addr,sizeof s_addr);
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(port);
  inet_pton(AF_INET,ipaddr,&(s_addr.sin_addr));

  int fd = socket(AF_INET,SOCK_STREAM,0);
  set_fd(fd);
  ::bind(get_fd(),(struct sockaddr*)&s_addr,sizeof s_addr);
  ::listen(get_fd,qlen);
  set_events(EPOLLIN);
  registerToLoop();
}

<template typename T>
Acceptor::close()
{

}

<template typename T>
void Acceptor::OnEvent() {
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
      Handler* pHandler = new T(cfd,loop_);
      pHandler.set_events(EPOLLIN|EPOLLOUT);
      pHandler.registerToLoop();
  }
}
