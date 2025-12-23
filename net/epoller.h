#ifndef UNITARY_NET_EPOLLER_H
#define UNITARY_NET_EPOLLER_H

#include "../base/common.h"
#include <sys/epoll.h>
#include <vector>

namespace unitary
{
class Channel;
class Epoller
{
  private:
	int _epfd;
	epoll_event* _events;

  public:
	DISABLE_COPY_AND_MOVE(Epoller)

	Epoller();
	~Epoller();

	void poll(std::vector<Channel*>* _chs, int timeout = -1);
	void updateChannel(Channel* ch);
	void deleteChannel(Channel* ch);
};
} // namespace unitary

#endif