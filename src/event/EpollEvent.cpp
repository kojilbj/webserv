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
	(void)ws;
	printLog(LOG_DEBUG, "timeOutHandler");
	std::list<struct eventData*>::iterator it = freeList.begin();
	int alignIndex = -1;
	bool timeout = false;
	for (; it != freeList.end(); it++)
	{
		if (!timeout)
			alignIndex++;
		else
		{
			if (alignIndex-- != 0)
				continue;
		}
		if ((*it)->type == ConnectionFd)
		{
			Protocol* p = (*it)->data.p;
#ifdef DEBUG
			std::stringstream fd;
			fd << p->c.cfd;
			if (p->c.lastReadTime == -1)
				printLog(LOG_DEBUG,
						 "connection fd (" + fd.str() + ") has 10 seconds left by timeout");
			else
			{
				std::stringstream left;
				left << 10 - (std::time(NULL) - p->c.lastReadTime);
				printLog(LOG_DEBUG,
						 "connection fd (" + fd.str() + ") has " + left.str() +
							 " seconds left by timeout");
			}
#endif
			// default client_request_timeout is 10s
			if (p->c.lastReadTime != -1 && std::time(NULL) - p->c.lastReadTime >= 10)
			{
				timeout = true;
				// close(p->c.cfd);
				// ws.getFreeList()->remove(p);
				// delete p;
				freeList.remove(*it);
				delete *it;
				Http* h = reinterpret_cast<Http*>(p);
				h->wevReady = true;
				int rv = h->createResponse("408");
				if (rv == AGAIN)
					h->revHandler = &Http::coreRunPhase;
				else
					h->revHandler = &Http::finalizeRequest;
				data_t data;
				data.p = p;
				ev->addEvent(h->c.cfd, data, ConnectionFd, MOD);
				h->c.lastReadTime = std::time(NULL);
				it = freeList.begin();
			}
		}
		else if ((*it)->type == UpstreamFd)
		{
			Upstream* upstream = (*it)->data.upstream;
#ifdef DEBUG
			if (upstream->lastReadTime == -1)
				printLog(LOG_DEBUG, "upstream fd has 10 seconds left by timeout");
			else
			{
				std::stringstream left;
				left << 10 - (std::time(NULL) - upstream->lastReadTime);
				printLog(LOG_DEBUG, "upstream fd has " + left.str() + " seconds left by timeout");
			}
#endif
			// default client_request_timeout is 10s
			if (upstream->lastReadTime != -1 && std::time(NULL) - upstream->lastReadTime >= 10)
			{
				timeout = true;
				if (upstream->writeFd != -1)
					close(upstream->writeFd);
				close(upstream->readFd);
				freeList.remove(*it);
				delete *it;
				Http* h = reinterpret_cast<Http*>(upstream->p);
				h->wevReady = true;
				int rv = h->createResponse("502");
				if (rv == AGAIN)
					h->revHandler = &Http::coreRunPhase;
				else
					h->revHandler = &Http::finalizeRequest;
				h->c.lastReadTime = std::time(NULL);
				data_t data;
				data.p = upstream->p;
				ev->addEvent(h->c.cfd, data, ConnectionFd, MOD);
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
	printLog(LOG_DEBUG, "waiting for next events ...");
	// timeout is 10s
	int events = epoll_wait(ep, eventResult, maxEvents, 10000);
#ifdef DEBUG
	std::stringstream num;
	num << events;
	printLog(LOG_DEBUG, "epoll_wait returned " + num.str() + " events");
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
		if (ed->type == ListeningFd)
		{
			Listening* ls = ed->data.ls;
#ifdef DEBUG
			std::stringstream fd;
			fd << ls->sfd;
			printLog(LOG_DEBUG, "listening fd (" + fd.str() + ") event occured");
			if (eventResult[i].events & EPOLLIN)
				printLog(LOG_DEBUG, "EPOLLIN");
			if (eventResult[i].events & EPOLLOUT)
				printLog(LOG_DEBUG, "EPOLLOUT");
			if (eventResult[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
				printLog(LOG_DEBUG, "EPOLLRDHUP | EPOLLHUP | EPOLLERR");
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
			std::stringstream fd;
			fd << p->c.cfd;
			printLog(LOG_DEBUG, "connection fd (" + fd.str() + ") event occured");
			if (eventResult[i].events & EPOLLIN)
				printLog(LOG_DEBUG, "EPOLLIN");
			if (eventResult[i].events & EPOLLOUT)
				printLog(LOG_DEBUG, "EPOLLOUT");
			if (eventResult[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
				printLog(LOG_DEBUG, "EPOLLRDHUP | EPOLLHUP | EPOLLERR");
#endif
			if (eventResult[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
			{
				printLog(LOG_DEBUG, "EPOLLRDHUP | EPOLLHUP | EPOLLERR returned, close connection");
				close(p->c.cfd);
				ws.getFreeList()->remove(p);
				delete p;
			}
			else if ((eventResult[i].events & EPOLLIN))
			{
				int rv = p->invokeRevHandler();
				if (rv == OK)
				{
					printLog(LOG_DEBUG, "revHandler returned OK, close connection");
					close(p->c.cfd);
					ws.getFreeList()->remove(p);
					delete p;
				}
				else if (rv == AGAIN)
				{
					printLog(LOG_DEBUG, "revHandler returned AGAIN, go back to epoll_wait");
					data_t data;
					data.p = p;
					ev->addEvent(p->c.cfd, data, ConnectionFd, MOD);
				}
				else if (rv == UPSTREAM_AGAIN)
				{
					printLog(
						LOG_DEBUG,
						"revHandler returned UPSTREAM_AGAIN, add upstream fd to epoll_wait and "
						"go back to epoll_wait");
					data_t data;
					data.upstream = p->upstream;
					ev->addEvent(p->upstream->writeFd, data, UpstreamFd, ADD);
					// data_t data2;
					// data2.p = p;
					// ev->addEvent(p->c.cfd, data2, ConnectionFd, MOD);
				}
				else
				{
					printLog(LOG_DEBUG, "revHandler returned ERROR, close connection");
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
					printLog(LOG_DEBUG, "revHandler returned OK, close connection");
					close(p->c.cfd);
					ws.getFreeList()->remove(p);
					delete p;
				}
				else if (rv == AGAIN)
				{
					printLog(LOG_DEBUG, "revHandler returned AGAIN, go back to epoll_wait");
					data_t data;
					data.p = p;
					ev->addEvent(p->c.cfd, data, ConnectionFd, MOD);
				}
				else
				{
					printLog(LOG_DEBUG, "revHandler returned ERROR, close connection");
					close(p->c.cfd);
					ws.getFreeList()->remove(p);
					delete p;
				}
			}
			freeList.remove(ed);
			delete ed;
		}
		else if (ed->type == UpstreamFd)
		{
#ifdef DEBUG
			printLog(LOG_DEBUG, "upstream fd event occured");
			if (eventResult[i].events & EPOLLIN)
				printLog(LOG_DEBUG, "EPOLLIN");
			if (eventResult[i].events & EPOLLOUT)
				printLog(LOG_DEBUG, "EPOLLOUT");
			if (eventResult[i].events & EPOLLRDHUP)
				printLog(LOG_DEBUG, "EPOLLRDHUP");
			if (eventResult[i].events & EPOLLHUP)
				printLog(LOG_DEBUG, "EPOLLHUP");
			if (eventResult[i].events & EPOLLERR)
				printLog(LOG_DEBUG, "EPOLLERR");
#endif
			Upstream* upstream = ed->data.upstream;
			if (eventResult[i].events & EPOLLOUT)
			{
				int rv = upstream->invokeRevHandler();
				if (rv == OK)
				{
					printLog(LOG_DEBUG, "upstream revHandler returned OK, close connection");
					close(upstream->writeFd);
					upstream->writeFd = -1;
					data_t data;
					data.upstream = upstream;
					ev->addEvent(upstream->readFd, data, UpstreamFd, ADD);
					// ws.getFreeList()->remove(p);
					// delete p;
				}
				else if (rv == AGAIN)
				{
					printLog(LOG_DEBUG,
							 "upstream revHandler returned AGAIN, go back to epoll_wait");
					data_t data;
					data.upstream = upstream;
					ev->addEvent(upstream->writeFd, data, UpstreamFd, MOD);
				}
				else // ERROR
				{
					printLog(LOG_DEBUG, "upstream revHandler returned ERROR, close connection");
					close(upstream->writeFd);
					close(upstream->readFd);
					data_t data;
					data.p = upstream->p;
					ev->addEvent(upstream->p->c.cfd, data, ConnectionFd, MOD);
					// finalizeRequest
				}
				freeList.remove(ed);
				delete ed;
			}
			else if (eventResult[i].events & (EPOLLIN | EPOLLHUP | EPOLLERR))
			{
				if (eventResult[i].events & (EPOLLHUP | EPOLLERR))
					upstream->peerClosed = true;
				int rv = upstream->invokeRevHandler();
				if (rv == OK)
				{
					printLog(LOG_DEBUG, "upstream revHandler returned OK, close connection");
					close(upstream->readFd);
					data_t data;
					data.p = upstream->p;
					ev->addEvent(upstream->p->c.cfd, data, ConnectionFd, MOD);
				}
				else if (rv == AGAIN)
				{
					printLog(LOG_DEBUG,
							 "upstream revHandler returned AGAIN, go back to epoll_wait");
					data_t data;
					data.upstream = upstream;
					ev->addEvent(upstream->readFd, data, UpstreamFd, MOD);
				}
				else // ERROR
				{
					printLog(LOG_DEBUG, "upstream revHandler returned ERROR, close connection");
					if (upstream->writeFd != -1)
						close(upstream->writeFd);
					close(upstream->readFd);
					data_t data;
					data.p = upstream->p;
					ev->addEvent(upstream->p->c.cfd, data, ConnectionFd, MOD);
				}
				freeList.remove(ed);
				delete ed;
			}
		}
		else
		{
#ifdef DEBUG
			std::cerr << "unknown event occured: " << ed->type << std::endl;
#endif
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
