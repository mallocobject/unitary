#include "address.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>

namespace unitary
{
Address::Address(const char* ip, const uint16_t port) : _port(port)
{
	strcpy(_ip, ip);
	::bzero(&_addr, sizeof(sockaddr_in));
	_addr.sin_family = AF_INET;
	::inet_pton(AF_INET, ip, &_addr.sin_addr);
	_addr.sin_port = ::htons(port);
}
} // namespace unitary