#include "EpollModule.hpp"

using namespace Webserv;

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
	std::vector<Listening>::const_iterator it;
	for (it = ws.getListenings()->begin(); it != ws.getListenings()->end(); it++)
	{
		eventList.events = EPOLLIN | EPOLLOUT;
		/* 'it' may be changed if you add some functionality to use addListening() */
		eventList->data.ptr = reinterpret_cast<void*>(&(*it));
		if (epoll_ctl(ep, EPOLL_CTL_ADD, it->sfd, &eventList) == -1)
		{
			std::cerr << strerror(errno) << std::endl;
			exit(1);
		}
	}
}

/* void Epoll::processEvents(Webserv& ws) */
/* { */
/* 	/1* this may be used by other event modules *1/ */
/* 	(void)ws; */
/* 	int events = epoll_wait(ep, eventList, maxEvents, 0); */
/* 	if (events == -1) */
/* 	{ */
/* 		std::cerr << strerror(errno) << std::endl; */
/* 		exit(1); */
/* 	} */
/* 	if (events == 0) */
/* 		return; */

/* 	Listening* ls; */
/* 	struct sockaddr_t sockaddr; */
/* 	socklen_t socklen = sizeof(struct sockaddr_t); */

/* 	for (int i = 0; i < events; i++) */
/* 	{ */
/* 		ls = reinterpret_cast<Listening*>(eventList[i].data.ptr); */
/* 		/1* this connection instance will be automatically distroyed when get out of this scope (next loop or out of loop) *1/ */
/* 		Connection c; */
/* 		c.setListening(ls); */
/* 		if (ls->protocol == "HTTP") */
/* 		{ */
/* 			/1* c.protocol = http; *1/ */
/* 			c.revHandler = reventHandler; */
/* 			c.wevHandler = weventHandler; */
/* 		} */
/* 		/1* else *1/ */
/* 		/1* { *1/ */
/* 		/1* } *1/ */
/* 		if (eventList[i].events & EPOLLIN) */
/* 		{ */
/* 			int cfd = accept(ls->fd, &sockaddr, &socklen); */
/* 			c.setAcceptRev(cfd, &sockaddr, socklen); */
/* 			/1* this call httpInitConnection() *1/ */
/* 			c.revHandler(c); */
/* 		} */
/* 		if (eventList[i].events & EPOLLOUT) */
/* 			c.wevHandler(c); */
/* 	} */
/* } */
