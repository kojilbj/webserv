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
	ep = epoll_create(1024); //ws.getListenings()->size() * 100);
	if (ep == -1)
	{
		std::string msg("epoll_create: ");
		msg += strerror(errno);
		throw std::string(msg);
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
			std::string msg("epoll_ctl: ");
			msg += strerror(errno);
			throw std::string(msg);
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
			{
				p->c.lastReadTime = std::time(NULL);
				printLog(LOG_DEBUG,
						 "connection fd (" + fd.str() + ") has 10 seconds left by timeout");
			}
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
				printLog(LOG_DEBUG, "timeout");
				timeout = true;
				struct eventData* tmp = *it;
				freeList.remove(*it);
				delete tmp;
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
				printLog(LOG_DEBUG, "upstream timeout");
				timeout = true;
				if (upstream->writeFd != -1)
				{
					close(upstream->writeFd);
					upstream->writeFd = -1;
				}
				close(upstream->readFd);
				upstream->readFd = -1;
				struct eventData* tmp = *it;
				freeList.remove(*it);
				delete tmp;
				Http* h = reinterpret_cast<Http*>(upstream->p);
				kill(h->getChildPid(), SIGKILL);
				h->wevReady = true;
				int rv = h->createResponse("502");
				if (rv == AGAIN)
					h->revHandler = &Http::coreRunPhase;
				else
					h->revHandler = &Http::finalizeRequest;
				h->c.lastReadTime = std::time(NULL);
				data_t data;
				data.p = upstream->p;
				delete data.p->upstream;
				data.p->upstream = NULL;
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
#ifdef DEBUG
	static int count = 0;
	printLog(LOG_DEBUG, "waiting for next events ...");
#endif
	int maxEvents = 5;
	struct epoll_event eventResult[maxEvents];
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
		// sometimes closed fd event occured, so check whether it is closed.
		if (!ed || std::find(freeList.begin(), freeList.end(), ed) == freeList.end())
			continue;
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
				continue;
		}
		else if (ed->type == ConnectionFd)
		{
			int rv = 0;
			Protocol* p = ed->data.p;
#ifdef DEBUG
			std::stringstream fd;
			fd << p->c.cfd;
			printLog(LOG_DEBUG, "connection fd (" + fd.str() + ") event occured");
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
			if (eventResult[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
			{
				std::stringstream fd;
				fd << p->c.cfd;
				printLog(LOG_DEBUG,
						 "EPOLLRDHUP | EPOLLHUP | EPOLLERR returned, close connection (" +
							 fd.str() + ")");
				Http* h = reinterpret_cast<Http*>(p);
				if (h->getRequestBodyFileFd() != -1)
					h->closeRequestBodyFileFd();
				if (access(h->requestBodyFileName_.c_str(), F_OK) != -1)
					std::remove(h->requestBodyFileName_.c_str());
				if (h->getResponseBodyFileFd() != -1)
					h->closeResponseBodyFileFd();
				if (access(h->responseBodyFileName_.c_str(), F_OK) != -1)
					std::remove(h->responseBodyFileName_.c_str());
#ifdef DEBUG
				std::stringstream ss;
				ss << ++count;
				printLog(LOG_DEBUG, "num of closed connection so far: " + ss.str());
#endif
				close(p->c.cfd);
				ws.getFreeList()->remove(p);
				delete p;
			}
			else if ((eventResult[i].events & EPOLLIN))
			{
			    rv = p->invokeRevHandler();
				if (rv == OK)
				{
					std::stringstream fd;
					fd << p->c.cfd;
					printLog(LOG_DEBUG,
							 "revHandler returned OK, close connection (" + fd.str() + ")");
					Http* h = reinterpret_cast<Http*>(p);
					if (h->getRequestBodyFileFd() != -1)
						h->closeRequestBodyFileFd();
					if (access(h->requestBodyFileName_.c_str(), F_OK) != -1)
						std::remove(h->requestBodyFileName_.c_str());
					if (h->getResponseBodyFileFd() != -1)
						h->closeResponseBodyFileFd();
					if (access(h->responseBodyFileName_.c_str(), F_OK) != -1)
						std::remove(h->responseBodyFileName_.c_str());
#ifdef DEBUG
					std::stringstream ss;
					ss << ++count;
					printLog(LOG_DEBUG, "num of closed connection so far: " + ss.str());
#endif
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

					data_t data2;
					data2.p = NULL;
					ev->addEvent(p->c.cfd, data2, ConnectionFd, MOD);
				}
				else
				{
					std::stringstream fd;
					fd << p->c.cfd;
					printLog(LOG_DEBUG,
							 "revHandler returned ERROR, close connection (" + fd.str() + ")");
					Http* h = reinterpret_cast<Http*>(p);
					if (h->getRequestBodyFileFd() != -1)
						h->closeRequestBodyFileFd();
					if (access(h->requestBodyFileName_.c_str(), F_OK) != -1)
						std::remove(h->requestBodyFileName_.c_str());
					if (h->getResponseBodyFileFd() != -1)
						h->closeResponseBodyFileFd();
					if (access(h->responseBodyFileName_.c_str(), F_OK) != -1)
						std::remove(h->responseBodyFileName_.c_str());
#ifdef DEBUG
					std::stringstream ss;
					ss << ++count;
					printLog(LOG_DEBUG, "num of closed connection so far: " + ss.str());
#endif
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
					std::stringstream fd;
					fd << p->c.cfd;
					printLog(LOG_DEBUG,
							 "revHandler returned OK, close connection (" + fd.str() + ")");
					Http* h = reinterpret_cast<Http*>(p);
					if (h->getRequestBodyFileFd() != -1)
						h->closeRequestBodyFileFd();
					if (access(h->requestBodyFileName_.c_str(), F_OK) != -1)
						std::remove(h->requestBodyFileName_.c_str());
					if (h->getResponseBodyFileFd() != -1)
						h->closeResponseBodyFileFd();
					if (access(h->responseBodyFileName_.c_str(), F_OK) != -1)
						std::remove(h->responseBodyFileName_.c_str());
#ifdef DEBUG
					std::stringstream ss;
					ss << ++count;
					printLog(LOG_DEBUG, "num of closed connection so far: " + ss.str());
#endif
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
					
					data_t data2;
					data2.p = NULL;
					ev->addEvent(p->c.cfd, data2, ConnectionFd, MOD);
				}
				else
				{
					std::stringstream fd;
					fd << p->c.cfd;
					printLog(LOG_DEBUG,
							 "revHandler returned ERROR, close connection (" + fd.str() + ")");
					Http* h = reinterpret_cast<Http*>(p);
					if (h->getRequestBodyFileFd() != -1)
						h->closeRequestBodyFileFd();
					if (access(h->requestBodyFileName_.c_str(), F_OK) != -1)
						std::remove(h->requestBodyFileName_.c_str());
					if (h->getResponseBodyFileFd() != -1)
						h->closeResponseBodyFileFd();
					if (access(h->responseBodyFileName_.c_str(), F_OK) != -1)
						std::remove(h->responseBodyFileName_.c_str());
#ifdef DEBUG
					std::stringstream ss;
					ss << ++count;
					printLog(LOG_DEBUG, "num of closed connection so far: " + ss.str());
#endif
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
					upstream->writeFd = -1;
					close(upstream->readFd);
					upstream->readFd = -1;
					data_t data;
					data.p = upstream->p;
					ev->addEvent(upstream->p->c.cfd, data, ConnectionFd, MOD);
				}
				freeList.remove(ed);
				delete ed;
			}
			else if (eventResult[i].events & (EPOLLIN | EPOLLHUP | EPOLLERR))
			{
				if (eventResult[i].events & (EPOLLHUP | EPOLLERR))
					upstream->peerClosed = true;
				if (eventResult[i].events & EPOLLIN)
					upstream->in = true;
				int rv = upstream->invokeRevHandler();
				upstream->in = false;
				if (rv == OK)
				{
					printLog(LOG_DEBUG, "upstream revHandler returned OK, close connection");
					close(upstream->readFd);
					upstream->readFd = -1;
					data_t data;
					data.p = upstream->p;
					ev->addEvent(upstream->p->c.cfd, data, ConnectionFd, MOD);
					//keepConnection = false;
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
					{
						close(upstream->writeFd);
						upstream->writeFd = -1;
					}
					close(upstream->readFd);
					upstream->readFd = -1;
					data_t data;
					data.p = upstream->p;
					ev->addEvent(upstream->p->c.cfd, data, ConnectionFd, MOD);
				}
				freeList.remove(ed);
				delete ed;
			}
		}
		else
			printLog(LOG_DEBUG, "unknown event occured");
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
	struct eventData* ed;
	if (type == ConnectionFd && !data.p)
		ed = NULL;
	else
	{
		ed = new struct eventData;
		ed->type = type;
		if (type == ConnectionFd)
			ed->data.p = data.p;
		else // UpstreamFd
			ed->data.upstream = data.upstream;
	}
	eventList.data.ptr = reinterpret_cast<void*>(ed);
	if (ed)
		freeList.push_back(ed);
	if (epoll_ctl(ep, op, fd, &eventList) == -1)
	{
		std::string msg("epoll_ctl: ");
		msg += strerror(errno);
		throw std::string(msg);
	}
}
