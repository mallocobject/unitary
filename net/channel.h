#ifndef UNITARY_NET_CHANNEL_H
#define UNITARY_NET_CHANNEL_H

#include "../base//common.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <sys/epoll.h>

namespace unitary
{
class EventLoop;
class Channel
{
  private:
	int _fd;
	EventLoop* _loop;
	bool _in_loop;
	uint32_t _events;
	uint32_t _revents;
	std::weak_ptr<void> _tie;
	bool _tied;

	std::function<void()> _read_callback;
	std::function<void()> _write_callback;
	std::function<void()> _close_callback;
	std::function<void()> _error_callback;

	void handleWithGuard();

  public:
	DISABLE_DEFAULT(Channel)
	DISABLE_COPY_AND_MOVE(Channel)

	Channel(EventLoop* loop, int fd)
		: _loop(loop), _fd(fd), _events(0), _revents(0), _in_loop(false),
		  _tied(false)
	{
	}

	~Channel() = default;

	void enableReading();
	void enableWriting();
	void useET();
	void disableReading();
	void disableWriting();
	void disableAll();
	void remove();

	bool isWriting() const
	{
		return _events & EPOLLOUT;
	}

	bool isReading() const
	{
		return _events & (EPOLLIN | EPOLLPRI);
	}

	int fd() const
	{
		return _fd;
	}

	void setRevents(uint32_t cv)
	{
		_revents = cv;
	}

	uint32_t getRevents() const
	{
		return _revents;
	}

	uint32_t getEvents() const
	{
		return _events;
	}

	bool inLoop() const
	{
		return _in_loop;
	}

	void setInLoop(bool on)
	{
		_in_loop = true;
	}

	// 防止执行TcpConnection的成员函数时，TcpConnection实例已被销毁
	void tie(const std::weak_ptr<void>& obj);
	void setTied()
	{
		_tied = true;
	}

	void setReadCallback(const std::function<void()>& cb)
	{
		_read_callback = cb;
	}

	void setWriteCallback(const std::function<void()>& cb)
	{
		_write_callback = cb;
	}

	void setCloseCallback(const std::function<void()>& cb)
	{
		_close_callback = cb;
	}

	void setErrorCallback(const std::function<void()>& cb)
	{
		_error_callback = cb;
	}

	void handle();
};
} // namespace unitary

#endif