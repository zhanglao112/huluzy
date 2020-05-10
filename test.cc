#include "tcpServer.h"

int main() {
	char ip[] = "127.0.0.1";
	TcpServer<TcpServerHandler> ts(ip,4999);
	ts.start();
}
