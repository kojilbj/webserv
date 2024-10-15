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
	int maxEvents = 5;
	struct epoll_event eventResult[maxEvents];
	int events = epoll_wait(ep, eventResult, maxEvents, -1);
	std::cout << events << std::endl;
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
		c.ls = ls;
		Protocol* p = NULL;
		if (ls->protocol == "HTTP")
		{
			/* p must be dynamically allocated */
			p = new Http;
			p->getServerCtx(ws.getConfCtxs(), ls);
		}
		else
		{
			/* you can add protocol (ex. mail, stream in nginx) */
			std::cerr << "no such protocol" << std::endl;
			exit(1);
		}
		if (eventResult[i].events & EPOLLIN) // EPOLLOUT ???
		{
			int cfd = accept(ls->sfd, (struct sockaddr*)&sockaddrIn, &socklen);
			if (cfd == -1)
			{
				std::cerr << strerror(errno) << std::endl;
				exit(1);
			}
			fcntl(cfd, F_SETFL, fcntl(cfd, F_GETFL) | O_NONBLOCK);
			c.setAcceptRev(cfd, &sockaddrIn, socklen);
			p->revHandler(c);
			close(cfd);
		}
		if (eventResult[i].events & (EPOLLHUP | EPOLLERR))
		{
			std::cout << "error!!!" << std::endl;
			exit(1);
		}
		delete p;
	}
}
