#ifndef UNITARY_NET_SOCKET_H
#define UNITARY_NET_SOCKET_H

#include "../base/common.h"
#include <unistd.h>

namespace unitary
{
class Address;
class Socket
{
  private:
	int _fd;

  public:
	DISABLE_DEFAULT(Socket)
	DISABLE_COPY(Socket)

	Socket(int fd) : _fd(fd)
	{
	}

	~Socket()
	{
		::close(_fd);
	}

	int fd() const
	{
		return _fd;
	}

	void bind(const Address& addr);
	void listen();
	int accept(Address* peer_addr);
	void connect(const Address& addr);

	void shutdownWrite();
	void setNoDelay(bool on);
	void setReuseAddr(bool on);
	void setReusePort(bool on);
	void setKeepAlive(bool on);
};
} // namespace unitary

#endif