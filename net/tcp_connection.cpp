#include "tcp_connection.h"
#include "address.h"
#include "channel.h"
#include "event_loop.h"
#include "socket.h"
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>

namespace unitary
{
TcpConnection::TcpConnection(EventLoop* loop, int fd, const char* ip,
							 uint16_t port)
	: _loop(loop)
{
	_sock = std::make_unique<Socket>(fd);
	_addr = std::make_unique<Address>(ip, port);
	_ch = std::make_unique<Channel>(loop, fd);
	_ch->setReadCallback(std::bind(&TcpConnection::handleRead, this));
	_ch->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
	_ch->setCloseCallback(std::bind(&TcpConnection::handleError, this));
	_ch->setErrorCallback(std::bind(&TcpConnection::handleError, this));
	_sock->setKeepAlive(true);
}

const char* TcpConnection::ip() const
{
	return _addr->ip();
}

uint16_t TcpConnection::port() const
{
	return _addr->port();
}

void TcpConnection::establishConnection()
{
	assert(_loop->isLocalThread());
	_ch->tie(weak_from_this());
	_ch->enableReading();
	_ch->useET();
	_connect_callback(shared_from_this());
}

void TcpConnection::destoryConnection()
{
	assert(_loop->isLocalThread());
	_ch->disableAll();
	_ch->remove();
}

void TcpConnection::shutdown()
{
	_loop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
}

void TcpConnection::shutdownInLoop()
{
	assert(_loop->isLocalThread());
	if (!_ch->isWriting())
	{
		_sock->shutdownWrite();
	}
}

} // namespace unitary