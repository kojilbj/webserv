#include "EpollEvent.hpp"

using namespace Wbsv;

class Epoll epoll;
Event* ev = &epoll;

void Epoll::init(Webserv& ws)
{
	ep = epoll_create(ws.getListenings()->size());
	if (ep == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		exit(1);
	}
	std::vector<Listening>::iterator it;
	for (it = ws.getListenings()->begin(); it != ws.getListenings()->end(); it++)
	{
		eventList.events = EPOLLIN | EPOLLOUT | EPOLLET;
		/* 'it' may be changed if you add some functionality to use addListening() */
		eventList.data.ptr = reinterpret_cast<void*>(&(*it));
		if (epoll_ctl(ep, EPOLL_CTL_ADD, it->sfd, &eventList) == -1)
		{
			std::cerr << strerror(errno) << std::endl;
			exit(1);
		}
	}
}

void Epoll::processEvents(Webserv& ws)
{
	/* this may be used by other event modules */
	(void)ws;
	int maxEvents = 5;
	struct epoll_event eventResult[maxEvents];
	int events = epoll_wait(ep, eventResult, maxEvents, 0);
	if (events == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		exit(1);
	}
	if (events == 0)
		return;

	Listening* ls;
	struct sockaddr_in sockaddrIn;
	socklen_t socklen = sizeof(struct sockaddr_in);

	for (int i = 0; i < events; i++)
	{
		ls = reinterpret_cast<Listening*>(eventResult[i].data.ptr);
		/* this connection instance will be automatically distroyed when get out of this scope (next loop or out of loop) */
		Connection c;
		c.setListening(ls);
		Protocol* p = NULL;
		if (ls->protocol == "HTTP")
		{
			/* p must be dynamically allocated */
			p = new Http;
		}
		else
		{
			/* you can add protocol (ex. mail, stream in nginx) */
			std::cerr << "no such protocol" << std::endl;
			exit(1);
		}
		if (eventResult[i].events & EPOLLIN)
		{
			int cfd = accept(ls->sfd, (struct sockaddr*)&sockaddrIn, &socklen);
			if (cfd == -1)
			{
				std::cerr << strerror(errno) << std::endl;
				exit(1);
			}
			/* nonblocking(cfd) */
			c.setAcceptRev(cfd, &sockaddrIn, socklen);
			/* this call httpInitConnection() */
			p->revHandler(c);
		}
		/* if (eventList[i].events & EPOLLOUT) */
		/* 	c.wevHandler(c); */
		delete p;
	}
}
