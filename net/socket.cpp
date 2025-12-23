#include "socket.h"
#include "../base/utils.h"
#include "address.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

namespace unitary
{
void Socket::bind(const Address& addr)
{
	erro(::bind(_fd, addr.getSockAddr(), sizeof(sockaddr)) == -1);
}

void Socket::listen()
{
	erro(::listen(_fd, SOMAXCONN) == -1);
}

int Socket::accept(Address* peer_addr)
{
	sockaddr_in addr;
	socklen_t len;
	int conn_fd = ::accept4(_fd, reinterpret_cast<sockaddr*>(&addr), &len,
							SOCK_NONBLOCK | SOCK_CLOEXEC);
	erro(conn_fd == -1);
	peer_addr->setSockAddr(addr);
	return conn_fd;
}

void Socket::connect(const Address& addr)
{
	erro(::connect(_fd, addr.getSockAddr(), sizeof(sockaddr)) == -1);
}

void Socket::shutdownWrite()
{
	erro(::shutdown(_fd, SHUT_WR) == -1);
}

void Socket::setNoDelay(bool on)
{
	int optval = on ? 1 : 0;
	erro(::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval)) ==
		 -1);
}

void Socket::setReuseAddr(bool on)
{
	int optval = on ? 1 : 0;
	erro(::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
		 -1);
}

void Socket::setReusePort(bool on)
{
	int optval = on ? 1 : 0;
	erro(::setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) ==
		 -1);
}

void Socket::setKeepAlive(bool on)
{
	int optval = on ? 1 : 0;
	erro(::setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) ==
		 -1);
}
} // namespace unitary