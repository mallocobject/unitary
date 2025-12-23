#ifndef UNITARY_NET_ADDRESS_H
#define UNITARY_NET_ADDRESS_H

#include <arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>

namespace unitary
{
class Address
{
  private:
	char _ip[INET_ADDRSTRLEN];
	uint16_t _port;
	sockaddr_in _addr;

  public:
	Address() = default;
	Address(const char* ip, uint16_t port);

	const char* ip() const
	{
		return _ip;
	}

	uint16_t port() const
	{
		return _port;
	}

	const sockaddr* getSockAddr() const
	{
		return reinterpret_cast<const sockaddr*>(&_addr);
	}

	void setSockAddr(const sockaddr_in& addr)
	{
		_addr = addr;
		::inet_ntop(AF_INET, &addr.sin_addr, _ip, sizeof(_ip));
		_port = ::ntohs(addr.sin_port);
	}
};
} // namespace unitary

#endif