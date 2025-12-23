#ifndef UNITARY_NET_TCP_CONNECTION_H
#define UNITARY_NET_TCP_CONNECTION_H

#include "../base/common.h"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace unitary
{
class Socket;
class Address;
class Channel;
class EventLoop;
class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
  private:
	std::unique_ptr<Socket> _sock;
	std::unique_ptr<Address> _addr;
	std::unique_ptr<Channel> _ch;
	EventLoop* _loop;

	std::function<void(const std::shared_ptr<TcpConnection>&)>
		_message_callback;
	std::function<void(const std::shared_ptr<TcpConnection>&)>
		_write_complete_callback;
	std::function<void(const std::shared_ptr<TcpConnection>&)> _close_callback;
	std::function<void(const std::shared_ptr<TcpConnection>&)>
		_connect_callback;

	void handleRead();
	void handleWrite();
	void handleClose();
	void handleError();
	void shutdownInLoop();

  public:
	DISABLE_DEFAULT(TcpConnection)
	DISABLE_COPY_AND_MOVE(TcpConnection)

	TcpConnection(EventLoop* loop, int fd, const char* ip, uint16_t port);
	~TcpConnection() = default;

	EventLoop* getLoop() const
	{
		return _loop;
	}

	const char* ip() const;
	uint16_t port() const;

	void setMessageCallback(
		const std::function<void(const std::shared_ptr<TcpConnection>&)>& cb)
	{
		_message_callback = cb;
	}

	void setWriteCompleteCallback(
		const std::function<void(const std::shared_ptr<TcpConnection>&)>& cb)
	{
		_write_complete_callback = cb;
	};

	void setCloseCallback(
		const std::function<void(const std::shared_ptr<TcpConnection>&)>& cb)
	{
		_close_callback = cb;
	}

	void setConnectCallback(
		const std::function<void(const std::shared_ptr<TcpConnection>&)>& cb)

	{
		_connect_callback = cb;
	}

	void establishConnection();
	void destoryConnection();
	void shutdown();

	void send(const std::string& message);
	void send(const char* message, size_t len);
};
} // namespace unitary

#endif