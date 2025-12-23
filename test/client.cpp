#include "../net/address.h"
#include "../net/socket.h"
#include <cstdio>
#include <iostream>
#include <ostream>
#include <sys/socket.h>
#include <unistd.h>

using namespace unitary;

int main()
{
	Address addr("127.0.0.1", 8888);
	Address client_addr("127.0.0.1", 6666);
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	Socket sock(fd);
	sock.setNoDelay(true);
	sock.setKeepAlive(true);
	sock.setReuseAddr(true);
	sock.setReusePort(true);
	sock.bind(client_addr);

	sock.connect(addr);

	printf("connect server(%s:%d)\n", addr.ip(), addr.port());

	std::string str;
	std::cout << "input: " << std::flush;

	std::cin >> str;

	ssize_t nbytes = ::write(fd, str.c_str(), str.size());

	if (nbytes > 0)
	{
		printf("has send %ld bytes\n", nbytes);
	}
	else if (nbytes == 0)
	{
		printf("server has closed\n");
	}
	else
	{
		printf("error occupid\n");
	}
}