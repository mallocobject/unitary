#include "../net/address.h"
#include "../net/socket.h"
#include <cstdio>
#include <cstring>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace unitary;

int main()
{
	Address addr("127.0.0.1", 8888);
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	Socket sock(fd);
	sock.setNoDelay(true);
	sock.setKeepAlive(true);
	sock.setReuseAddr(true);
	sock.setReusePort(true);
	sock.bind(addr);
	sock.listen();
	Address peer_addr;
	int conn_fd = sock.accept(&peer_addr);
	printf("connection from %s:%d\n", peer_addr.ip(), peer_addr.port());

	char buf[1024];
	bzero(buf, sizeof(buf));

	ssize_t nbytes = ::read(conn_fd, buf, sizeof(buf));

	if (nbytes > 0)
	{
		printf("client: %s\n", buf);
	}
	else if (nbytes == 0)
	{
		printf("read EOF\n");
	}
	else
	{
		printf("error occupid\n");
	}
}