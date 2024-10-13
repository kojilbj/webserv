#include "EpollModule.hpp"

using namespace Wbsv;

class Epoll epoll;
Module* ev = &epoll;

/* extern reventHandler; */
/* extern weventHandler; */

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
		eventList.events = EPOLLIN | EPOLLOUT;
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
	struct sockaddr sockaddr;
	socklen_t socklen = sizeof(struct sockaddr);

	for (int i = 0; i < events; i++)
	{
		ls = reinterpret_cast<Listening*>(eventResult[i].data.ptr);
		/* this connection instance will be automatically distroyed when get out of this scope (next loop or out of loop) */
		/* Connection c; */
		/* c.setListening(ls); */
		/* if (ls->protocol == "HTTP") */
		/* { */
		/* 	/1* c.protocol = http; *1/ */
		/* 	c.revHandler = reventHandler; */
		/* 	c.wevHandler = weventHandler; */
		/* } */
		/* else */
		/* { */
		/* } */
		if (eventResult[i].events & EPOLLIN)
		{
			int cfd = accept(ls->sfd, &sockaddr, &socklen);
			/* c.setAcceptRev(cfd, &sockaddr, socklen); */
			/* this call httpInitConnection() */
			/* c.revHandler(c); */
			int bufsize = 1024;
			char buf[bufsize];
			ssize_t readnum = read(cfd, buf, bufsize);
			if (readnum < 0)
				std::cout << "read error" << std::endl;
			else
			{
				buf[readnum] = '\0';
				std::cout << buf << std::endl;
			}
		}
		/* if (eventList[i].events & EPOLLOUT) */
		/* 	c.wevHandler(c); */
	}
}
