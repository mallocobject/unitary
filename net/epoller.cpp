#include "epoller.h"
#include "../base/utils.h"
#include "channel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

namespace unitary
{

#define MAX_EVENTS 1024

Epoller::Epoller() : _events(new epoll_event[MAX_EVENTS])
{
	_epfd = epoll_create1(0);
	erro(_epfd == -1);
}

Epoller::~Epoller()
{
	::close(_epfd);
	delete[] _events;
	_events = nullptr;
}

void Epoller::poll(std::vector<Channel*>* _chs, int timeout)
{
	int nfds = epoll_wait(_epfd, _events, MAX_EVENTS, timeout);
	erro(nfds == -1);
	_chs->resize(nfds);
	for (int i = 0; i < nfds; i++)
	{
		(*_chs)[i] = reinterpret_cast<Channel*>(_events->data.ptr);
		(*_chs)[i]->setRevents(_events->events);
	}
}

void Epoller::updateChannel(Channel* ch)
{
	int fd = ch->fd();
	epoll_event cv;
	cv.events = ch->getRevents();
	cv.data.ptr = ch;
	if (ch->inLoop())
	{
		erro(epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &cv) == -1);
	}
	else
	{
		erro(epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &cv) == -1);
		ch->setInLoop(true);
	}
}

void Epoller::deleteChannel(Channel* ch)
{
	int fd = ch->fd();
	epoll_event cv;
	cv.events = ch->getRevents();
	cv.data.ptr = ch;
	if (ch->inLoop())
	{
		erro(epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr) == -1);
		ch->setInLoop(false);
	}
}
} // namespace unitary