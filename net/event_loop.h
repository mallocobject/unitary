#ifndef UNITARY_NET_EVENT_LOOP_H
#define UNITARY_NET_EVENT_LOOP_H

#include "../base/common.h"
#include <atomic>
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>

namespace unitary
{
class Epoller;
class Channel;
class EventLoop
{
  private:
	std::unique_ptr<Epoller> _epoll;
	std::unique_ptr<Channel> _wakeup_ch;
	int _wakeup_fd;
	std::atomic<bool> _is_quit;
	std::vector<Channel*> _chs;
	std::thread::id _tid;
	std::vector<std::function<void()>> _funcs;
	std::atomic<bool> _calling_func;
	mutable std::mutex _mtx;

	void wakeup()
	{
		uint64_t signal = 1;
		ssize_t nbytes = ::write(_wakeup_fd, &signal, sizeof(signal));
		assert(nbytes == sizeof(signal));
	}

	void handleRead()
	{
		uint64_t signal = 1;
		ssize_t nbytes = ::read(_wakeup_fd, &signal, sizeof(signal));
		assert(nbytes == sizeof(signal));
	}

	void doFuncs();

  public:
	DISABLE_COPY_AND_MOVE(EventLoop)

	bool isLocalThread() const
	{
		return _tid == std::this_thread::get_id();
	}

	EventLoop();
	~EventLoop();

	void loop();
	void runInLoop(const std::function<void()>& cb);
	void queueInLoop(const std::function<void()>& cb);
	void updateChannel(Channel* ch);
	void deleteChannel(Channel* ch);
};
} // namespace unitary

#endif