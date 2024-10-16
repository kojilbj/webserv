#include "EpollEvent.hpp"

using namespace Wbsv;

class Epoll epoll;
Event* ev = &epoll;

Epoll::Epoll()
	: ep(-1)
{
	eventList.events = 0;
	eventList.data.ptr = NULL;
}

Epoll::~Epoll() { }

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
		eventData* ed = new eventData;
		ed->type = "LISTENING";
		ed->data.ls = &(*it);
		eventList.data.ptr = reinterpret_cast<void*>(ed);
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
	for (int i = 0; i < events; i++)
	{
		/* this data pointer must be freed after use */
		eventData* ed = reinterpret_cast<eventData*>(eventResult[i].data.ptr);
		if (ed->type == "LISTENING")
		{
			std::cout << "type: " << ed->type << std::endl;
			Listening* ls = ed->data.ls;
			if (eventResult[i].events & EPOLLIN) // EPOLLOUT ???
				ws.acceptEvent(ls);
			delete ed;
			if (eventResult[i].events & (EPOLLHUP | EPOLLERR))
			{
				std::cout << "EPOLLHUP or EPOLLERR" << std::endl;
				exit(1);
			}
		}
		else if (ed->type == "PROTOCOL")
		{
			if (eventResult[i].events & EPOLLOUT)
				break;
			if (eventResult[i].events & EPOLLIN)
				std::cout << "EPOLLIN" << std::endl;
			if (eventResult[i].events & (EPOLLHUP | EPOLLERR))
				std::cout << "EPOLLHUP or EPOLLERR" << std::endl;
			std::cout << "type: " << ed->type << std::endl;
			Protocol* p = ed->data.p;
			p->revHandler();
			delete p;
			delete ed;
			close(p->c.cfd);
		}
		else
		{
			/* std::cerr << "unknown event" << std::endl; */
			break;
		}
	}
}

void Epoll::addEvent(int fd, Protocol* p)
{
	eventList.events = EPOLLIN | EPOLLOUT | EPOLLET;
	eventData* ed = new eventData;
	ed->type = "PROTOCOL";
	ed->data.p = p;
	eventList.data.ptr = reinterpret_cast<void*>(ed);
	if (epoll_ctl(ep, EPOLL_CTL_ADD, fd, &eventList) == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		exit(1);
	}
}
