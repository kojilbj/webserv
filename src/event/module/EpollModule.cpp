#include "EpollModule.hpp"

using namespace Webserv;

Epoll ev;

void Epoll::init(Webserv& ws)
{
	ep = epoll_create(10);
	epoll_ctl(ep, EPOLL_CTL_ADD, ...);
}

void Epoll::processEvents(Webserv& ws)
{
	int events = epoll_wait(ep, eventList, nevents, timer);
}
