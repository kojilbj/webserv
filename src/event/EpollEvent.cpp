#include "EpollEvent.hpp"

using namespace Wbsv;

extern sig_atomic_t sigInterrupt;

class Epoll epoll;
Event* ev = &epoll;

Epoll::Epoll()
	: ep(-1)
{ }

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
		struct epoll_event eventList;
		eventList.events = EPOLLIN | EPOLLOUT | EPOLLET;
		/* 'it' may be changed if you add some functionality to use addListening() */
		struct eventData* ed = new struct eventData;
		ed->type = ListeningFd;
		ed->data.ls = &(*it);
		eventList.data.ptr = reinterpret_cast<void*>(ed);
#ifdef DEBUG
		std::cout << "Before epoll_wait:" << std::endl;
		std::cout << "\teventData: " << ed << std::endl;
		std::cout << "\teventData->data.ls: " << ed->data.ls << std::endl;
#endif
		freeList.push_back(ed);
		if (epoll_ctl(ep, EPOLL_CTL_ADD, it->sfd, &eventList) == -1)
		{
			std::cerr << strerror(errno) << std::endl;
			exit(1);
		}
	}
}

void Epoll::processEventsLoop(Webserv& ws)
{
	for (;;)
	{
		ev->processEvents(ws);
		if (sigInterrupt)
		{
			std::cout << "Program is successfully closing" << std::endl;
			std::list<struct eventData*>::iterator it;
			for (it = freeList.begin(); it != freeList.end(); it++)
			{
				delete *it;
			}
			break;
		}
	}
}

void Epoll::processEvents(Webserv& ws)
{
	int maxEvents = 5;
	struct epoll_event eventResult[maxEvents];
#ifdef DEBUG
	std::cout << "Waiting for next events ..." << std::endl;
#endif
	int events = epoll_wait(ep, eventResult, maxEvents, -1);
#ifdef DEBUG
	std::cout << events << " events occured" << std::endl;
#endif
	if (events == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		return;
	}
	if (events == 0)
		return;
	for (int i = 0; i < events; i++)
	{
		/* this data pointer must be freed after use */
		struct eventData* ed = reinterpret_cast<struct eventData*>(eventResult[i].data.ptr);
#ifdef DEBUG
		std::cout << "After epoll_wait:" << std::endl;
		std::cout << "\teventData: " << ed << std::endl;
		std::cout << "\teventData->data.ls: " << ed->data.ls << std::endl;
#endif
		if (ed->type == ListeningFd)
		{
			Listening* ls = ed->data.ls;
#ifdef DEBUG
			std::cout << "Listening socket event occured" << std::endl;
			std::cout << "fd: " << ls->sfd << std::endl;
			if (eventResult[i].events & EPOLLIN)
				std::cout << "event: EPOLLIN" << std::endl;
			if (eventResult[i].events & EPOLLOUT)
				std::cout << "event: EPOLLOUT" << std::endl;
			if (eventResult[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
				std::cout << "event: EPOLLERR" << std::endl;
			std::cout << std::endl;
#endif
			if (eventResult[i].events & EPOLLIN)
				ws.acceptEvent(ls);
			if (eventResult[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
				break;
		}
		else if (ed->type == ConnectionFd)
		{
			Protocol* p = ed->data.p;
#ifdef DEBUG
			std::cout << "Connection socket event occured" << std::endl;
			std::cout << "fd: " << p->c.cfd << std::endl;
			if (eventResult[i].events & EPOLLIN)
				std::cout << "event: EPOLLIN" << std::endl;
			if (eventResult[i].events & EPOLLOUT)
				std::cout << "event: EPOLLOUT" << std::endl;
			if (eventResult[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
				std::cout << "event: EPOLLERR" << std::endl;
			std::cout << std::endl;
#endif
			if ((eventResult[i].events & EPOLLIN) && (eventResult[i].events & EPOLLOUT))
			{
				int rv = p->revHandler();
				if (rv == OK)
					close(p->c.cfd);
				else if (rv == AGAIN)
					ev->addEvent(p->cfd, p, EPOLL_CTL_MOD);
				freeList.remove(ed);
				delete ed;
			}
			if (eventResult[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
				break;
		}
		else
		{
			std::cerr << "unknown event" << std::endl;
			break;
		}
	}
}

void Epoll::addEvent(int fd, Protocol* p)
{
	struct epoll_event eventList;
	eventList.events = EPOLLIN | EPOLLOUT | EPOLLET;
	struct eventData* ed = new struct eventData;
	ed->type = ConnectionFd;
	ed->data.p = p;
	eventList.data.ptr = reinterpret_cast<void*>(ed);
	freeList.push_back(ed);
	if (epoll_ctl(ep, EPOLL_CTL_ADD, fd, &eventList) == -1)
	{
		std::cerr << strerror(errno) << std::endl;
		exit(1);
	}
}
