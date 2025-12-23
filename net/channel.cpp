#include "channel.h"
#include "event_loop.h"
#include <cstdio>
#include <memory>
#include <sys/epoll.h>

namespace unitary
{
void Channel::enableReading()
{
	_events |= EPOLLIN | EPOLLPRI;
	_loop->updateChannel(this);
}

void Channel::enableWriting()
{
	_events |= EPOLLOUT;
	_loop->updateChannel(this);
}

void Channel::useET()
{
	_events |= EPOLLET;
	_loop->updateChannel(this);
}

void Channel::disableReading()
{
	_events &= ~(EPOLLIN | EPOLLPRI);
	_loop->updateChannel(this);
}

void Channel::disableWriting()
{
	_events &= ~EPOLLOUT;
	_loop->updateChannel(this);
}

void Channel::disableAll()
{
	_events = 0;
	_loop->updateChannel(this);
}

void Channel::remove()
{
	_loop->deleteChannel(this);
}

void Channel::tie(const std::weak_ptr<void>& obj)
{
	_tie = obj;
	_tied = true;
}

void Channel::handle()
{
	std::shared_ptr<void> gurad = _tie.lock();
	if (gurad)
	{
		handleWithGuard();
	}
}

void Channel::handleWithGuard()
{
	if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
	{
		printf("Channel::handle EPOLLHUP");
		if (_close_callback)
		{
			_close_callback();
		}
	}
	if (_revents & EPOLLERR)
	{
		if (_error_callback)
		{
			_error_callback();
		}
	}
	if (_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
	{
		if (_read_callback)
		{
			_read_callback(); // 有主动断开功能
		}
	}
	if (_revents & EPOLLOUT)
	{
		if (_write_callback)
		{
			_write_callback();
		}
	}
}

} // namespace unitary