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
		std::cerr << "epoll_create: " << strerror(errno) << std::endl;
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
			std::cerr << "epoll_ctl: " << strerror(errno) << std::endl;
			exit(1);
		}
	}
}

void Epoll::timeOutHandler(Webserv& ws)
{
#ifdef DEBUG
	std::cout << "timeOutHandler" << std::endl;
#endif
	bool timout = false;
	std::list<struct eventData*>::iterator it = freeList.begin();
	for (; it != freeList.end(); it++)
	{
		if ((*it)->type == ConnectionFd)
		{
			Protocol* p = (*it)->data.p;
#ifdef DEBUG
			std::cout << "connection fd: " << p->c.cfd << std::endl;
			std::cout << "last read time: " << p->c.lastReadTime << std::endl;
			std::cout << "current time: " << std::time(NULL) << std::endl;
			std::cout << "diff: " << std::time(NULL) - p->c.lastReadTime << std::endl;
#endif
			// default client_request_timeout is 60s
			if (p->c.lastReadTime != -1 && std::time(NULL) - p->c.lastReadTime >= 10)
			{
				close(p->c.cfd);
				ws.getFreeList()->remove(p);
				delete p;
				freeList.remove(*it);
				delete *it;
				it = freeList.begin();
			}
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
			std::list<struct eventData*>::iterator it;
			for (it = freeList.begin(); it != freeList.end(); it++)
			{
				delete *it;
			}
			std::cout << "Program is successfully closed" << std::endl;
			break;
		}
		timeOutHandler(ws);
	}
}

void Epoll::processEvents(Webserv& ws)
{
	int maxEvents = 5;
	struct epoll_event eventResult[maxEvents];
#ifdef DEBUG
	std::cout << "Waiting for next events ..." << std::endl;
#endif
	// timeout is 10s
	int events = epoll_wait(ep, eventResult, maxEvents, 10000);
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
			std::cout << "upstream fd: " << p->c.upstreamFd << std::endl;
			if (eventResult[i].events & EPOLLIN)
				std::cout << "event: EPOLLIN" << std::endl;
			if (eventResult[i].events & EPOLLOUT)
				std::cout << "event: EPOLLOUT" << std::endl;
			if (eventResult[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
				std::cout << "event: EPOLLERR" << std::endl;
			std::cout << std::endl;
#endif
			if ((eventResult[i].events & EPOLLIN))
			{
				int rv = p->invokeRevHandler();
				if (rv == OK)
				{
					std::cout << "RevHandler finished, close connection" << std::endl;
					close(p->c.cfd);
					ws.getFreeList()->remove(p);
					delete p;
				}
				else if (rv == AGAIN)
				{
#ifdef DEBUG
					std::cout << "RevHandler return AGAIN, addEvent(MOD)" << std::endl;
#endif
					data_t data;
					data.p = p;
					ev->addEvent(p->c.cfd, data, ConnectionFd, MOD);
				}
				else if (rv == UPSTREAM_AGAIN)
				{
#ifdef DEBUG
					std::cout << "RevHandler return UPSTREAM_AGAIN, addEvent(ADD)" << std::endl;
#endif
					data_t data;
					data.upstream = p->upstream;
					ev->addEvent(p->upstream->writeFd, data, UpstreamFd, ADD);
					data_t data2;
					data2.p = p;
					ev->addEvent(p->c.cfd, data2, ConnectionFd, MOD);
				}
				else
				{
					std::cout << "Error occured while revHandler, close connection" << std::endl;
					close(p->c.cfd);
					ws.getFreeList()->remove(p);
					delete p;
				}
			}
			else if (eventResult[i].events & EPOLLOUT)
			{
				// this area does not work, when "curl -X POST -H "Content-Length: 100" --data "aaa" localhost:8080/upload.php"
				if (!p->wevReady) // or if (statusLine == "")
					continue;
				int rv = p->invokeRevHandler();
				if (rv == OK)
				{
					std::cout << "RevHandler finished, close connection" << std::endl;
					close(p->c.cfd);
					ws.getFreeList()->remove(p);
					delete p;
				}
				else if (rv == AGAIN)
				{
#ifdef DEBUG
					std::cout << "RevHandler return AGAIN, addEvent(MOD)" << std::endl;
#endif
					data_t data;
					data.p = p;
					ev->addEvent(p->c.cfd, data, ConnectionFd, MOD);
				}
				else
				{
					std::cout << "Error occured while revHandler, close connection" << std::endl;
					close(p->c.cfd);
					ws.getFreeList()->remove(p);
					delete p;
				}
			}
			else if (eventResult[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
			{
				std::cout << "EPOLLERR returned, close connection" << std::endl;
				close(p->c.cfd);
				ws.getFreeList()->remove(p);
				delete p;
			}
			freeList.remove(ed);
			delete ed;
		}
		else if (ed->type == UpstreamFd)
		{
			Upstream* upstream = ed->data.upstream;
			if (eventResult[i].events & EPOLLOUT)
			{
				int rv = upstream->invokeRevHandler();
				if (rv == OK)
				{
					std::cout << "Upstream revHandler finished, close connection" << std::endl;
					close(upstream->writeFd);
					data_t data;
					data.upstream = upstream;
					ev->addEvent(upstream->readFd, data, UpstreamFd, ADD);
					// ws.getFreeList()->remove(p);
					// delete p;
				}
				else if (rv == AGAIN)
				{
#ifdef DEBUG
					std::cout << "Upstream RevHandler return AGAIN, addEvent(MOD)" << std::endl;
#endif
					data_t data;
					data.upstream = upstream;
					ev->addEvent(upstream->writeFd, data, UpstreamFd, MOD);
				}
				else
				{
					std::cout << "Error occured while Upstream revHandler, close connection"
							  << std::endl;
					close(upstream->writeFd);
					close(upstream->readFd);
					// finalizeRequest
				}
				freeList.remove(ed);
				delete ed;
			}
			else if (eventResult[i].events & EPOLLIN)
			{
				int rv = upstream->invokeRevHandler();
				if (rv == OK)
				{
					std::cout << "Upstream revHandler finished, close connection" << std::endl;
					close(upstream->readFd);
					// upstream->p->revHandler = &Http::finalizeRequest;
				}
				else if (rv == AGAIN)
				{
#ifdef DEBUG
					std::cout << "Upstream RevHandler return AGAIN, addEvent(MOD)" << std::endl;
#endif
					data_t data;
					data.upstream = upstream;
					ev->addEvent(upstream->readFd, data, UpstreamFd, MOD);
				}
				else
				{
					std::cout << "Error occured while Upstream revHandler, close connection"
							  << std::endl;
					close(upstream->readFd);
				}
				freeList.remove(ed);
				delete ed;
			}
		}
		else
		{
			std::cerr << "unknown event: " << ed->type << std::endl;
			break;
		}
	}
}

void Epoll::addEvent(int fd, data_t& data, int type, int option)
{
	struct epoll_event eventList;
	eventList.events = EPOLLIN | EPOLLOUT | EPOLLET;
	int op;
	if (option == ADD)
		op = EPOLL_CTL_ADD;
	else // MOD
		op = EPOLL_CTL_MOD;
	struct eventData* ed = new struct eventData;
	ed->type = type;
	if (type == ConnectionFd)
		ed->data.p = data.p;
	else // UpstreamFd
		ed->data.upstream = data.upstream;
	eventList.data.ptr = reinterpret_cast<void*>(ed);
	freeList.push_back(ed);
	if (epoll_ctl(ep, op, fd, &eventList) == -1)
	{
		std::cout << fd << std::endl;
		std::cerr << "epoll_ctl: " << strerror(errno) << std::endl;
		exit(1);
	}
}
