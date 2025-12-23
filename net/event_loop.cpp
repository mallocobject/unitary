#include "event_loop.h"
#include "channel.h"
#include "epoller.h"
#include <functional>
#include <memory>
#include <mutex>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>

namespace unitary
{
void EventLoop::queueInLoop(const std::function<void()>& cb)
{
	{
		std::lock_guard<std::mutex> lock(_mtx);
		_funcs.push_back(cb);
	}
	// _calling_func: 在执行funcs数组时，调用了这个函数
	if (!isLocalThread() || _calling_func)
	{
		wakeup();
	}
}

void EventLoop::doFuncs()
{
	_calling_func = true;
	std::vector<std::function<void()>> funcs_copy;
	{
		std::lock_guard<std::mutex> lock(_mtx);
		std::swap(funcs_copy, _funcs);
	}
	for (auto& f : funcs_copy)
	{
		f();
	}
	_calling_func = false;
}

EventLoop::EventLoop() : _is_quit(false), _calling_func(false)
{
	_epoll = std::make_unique<Epoller>();
	_wakeup_fd = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
	_wakeup_ch = std::make_unique<Channel>(this, _wakeup_fd);

	_wakeup_ch->setReadCallback(std::bind(&EventLoop::handleRead, this));
	_wakeup_ch->enableReading();
}

EventLoop::~EventLoop()
{
	::close(_wakeup_fd);
}

void EventLoop::loop()
{
	_tid = std::this_thread::get_id();
	while (!_is_quit)
	{
		_epoll->poll(&_chs);
		for (Channel* ch : _chs)
		{
			ch->handle();
		}
		doFuncs();
	}
}

void EventLoop::runInLoop(const std::function<void()>& cb)
{
	if (isLocalThread())
	{
		cb();
	}
	else
	{
		queueInLoop(cb);
	}
}

void EventLoop::updateChannel(Channel* ch)
{
	_epoll->updateChannel(ch);
}

void EventLoop::deleteChannel(Channel* ch)
{
	_epoll->deleteChannel(ch);
}
} // namespace unitary