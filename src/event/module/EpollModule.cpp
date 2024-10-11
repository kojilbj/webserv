#include "EpollModule.hpp"

using namespace Webserv;

extern reventHandler;
extern weventHandler;

Epoll ev;

void Epoll::init(Webserv& ws)
{
	ep = epoll_create(ws.listenings.size());
	if (ep == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		exit(1);
	}

	std::vector<Listening>::iterator it;
	for (it = ws.listenings.begin(); it != ws.listening.end(); it++)
	{
		eventList.events = EPOLLIN | EPOLLOUT;
		/* 'it' may be changed if you add some functionality to use addListening() */
		eventList.data.ptr = reinterpret_cast<void*>(&(*it));
		epoll_ctl(ep, EPOLL_CTL_ADD, it->fd, eventList);
	}
}

void Epoll::processEvents(Webserv& ws)
{
	/* this may be used by other event modules */
	(void)ws;
	int events = epoll_wait(ep, eventList, maxEvents, 0);
	if (events == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		exit(1);
	}
	if (events == 0)
		return;

	Listening* ls;
	struct sockaddr_t sockaddr;
	socklen_t socklen = sizeof(struct sockaddr_t);

	for (int i = 0; i < events; i++)
	{
		ls = reinterpret_cast<Listening*>(eventList[i].data.ptr);
		/* this connection instance will be automatically distroyed when get out of this scope (next loop or out of loop) */
		Connection c;
		c.setListening(ls);
		if (ls->protocol == "HTTP")
		{
			/* c.protocol = http; */
			c.revHandler = reventHandler;
			c.wevHandler = weventHandler;
		}
		/* else */
		/* { */
		/* } */
		if (eventList[i].events & EPOLLIN)
		{
			int cfd = accept(ls->fd, &sockaddr, &socklen);
			c.setAcceptRev(cfd, &sockaddr, socklen);
			/* this call httpInitConnection() */
			c.revHandler(c);
		}
		if (eventList[i].events & EPOLLOUT)
			c.wevHandler(c);
	}
}
