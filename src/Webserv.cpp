#include "Webserv.hpp"

using namespace Wbsv;

extern Wbsv::Event* ev;

Webserv::Webserv()
	: confCtxs_(NULL), listenings_(NULL)
{ }

Webserv::~Webserv()
{
	std::vector<ConfCtx*>::iterator it;
	if (confCtxs_ && !confCtxs_->empty())
	{
		for (it = (*confCtxs_).begin(); it != (*confCtxs_).end(); it++)
		{
			delete *it;
		}
	}
	// delete confCtxs_;
	delete listenings_;
	std::list<Protocol*>::iterator pit;
	for (pit = freeList.begin(); pit != freeList.end(); pit++)
	{
		delete *pit;
	}
}

std::vector<ConfCtx*>* Webserv::getConfCtxs()
{
	return confCtxs_;
}

std::vector<Listening>* Webserv::getListenings()
{
	return listenings_;
}

std::list<Protocol*>* Webserv::getFreeList()
{
	return &freeList;
}

void Webserv::setConfCtxs(std::vector<ConfCtx*>* confCtxs)
{
	confCtxs_ = confCtxs;
}

void Webserv::init()
{
	listenings_ = new std::vector<Listening>;
	std::vector<ConfCtx*>::iterator it = (*confCtxs_).begin();
	for (it = (*confCtxs_).begin(); it != (*confCtxs_).end(); it++)
	{
		(*it)->initListening(listenings_);
	}
}

void Webserv::openListeningSocket()
{
	std::vector<Listening>::iterator it;
	int s;

	for (it = listenings_->begin(); it != listenings_->end(); it++)
	{
		s = socket(it->family, it->socktype, 0);
		if (s == -1)
		{
			std::string msg("socket: ");
			msg += strerror(errno);
			throw std::string(msg);
		}
		fcntl(s, F_SETFL, fcntl(s, F_GETFL) | O_NONBLOCK);
		int optval = 1;
		if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		{
			std::string msg("setsockopt: ");
			msg += strerror(errno);
			throw std::string(msg);
		}
		if (bind(s, (struct sockaddr*)&it->sockaddrIn, it->socklen) != 0)
		{
			std::string msg("bind: ");
			msg += strerror(errno);
			throw std::string(msg);
		}
		if (listen(s, it->backlog) == -1)
		{
			std::string msg("listen: ");
			msg += strerror(errno);
			throw std::string(msg);
		}
		it->sfd = s;
	}
}

void Webserv::acceptEvent(Listening* ls)
{
	struct sockaddr_in claddr;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	int cfd = accept(ls->sfd, (struct sockaddr*)&claddr, &addrlen);
#ifdef DEBUG
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	std::memset(host, 0, NI_MAXHOST);
	std::memset(service, 0, NI_MAXSERV);
	getnameinfo(
		(struct sockaddr*)&claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
	printLog(LOG_DEBUG, "accepted client (" + std::string(host) + ":" + std::string(service) + ")");
	static int count = 0;
	std::stringstream ss;
	ss << ++count;
	printLog(LOG_DEBUG, "num of accepted connection so far: " + ss.str());
#endif
	if (cfd == -1)
	{
		std::string msg("accept: ");
		msg += strerror(errno);
		throw std::string(msg);
	}
	fcntl(cfd, F_SETFL, fcntl(cfd, F_GETFL) | O_NONBLOCK);
	if (ls->protocol == "HTTP")
	{
		/* p must be dynamically allocated */
		Protocol* p = new Http;
		freeList.push_back(p);
		Http* h = reinterpret_cast<Http*>(p);
		h->revHandler = &Http::waitRequestHandler;
		p->selectServerCtx(getConfCtxs(), ls);
		p->initPhaseHandler();
		Connection* c = &p->c;
		c->ls = ls;
		c->setAcceptRev(cfd, &claddr, addrlen);
		data_t data;
		data.p = p;
		ev->addEvent(cfd, data, ConnectionFd, ADD);
	}
	else
	{
		/* you can add protocol (ex. mail, stream in nginx) */
		throw std::string("no such protocol");
	}
}
