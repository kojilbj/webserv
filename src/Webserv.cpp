#include "Webserv.hpp"

using namespace Wbsv;

extern Wbsv::Event* ev;

Webserv::Webserv()
	: confCtxs_(NULL), listenings_(NULL)
{ }

Webserv::~Webserv()
{
	std::vector<ConfCtx*>::iterator it;
	for (it = (*confCtxs_).begin(); it != (*confCtxs_).end(); it++)
	{
		delete *it;
	}
	delete confCtxs_;
	delete listenings_;
}

std::vector<ConfCtx*>* Webserv::getConfCtxs()
{
	return confCtxs_;
}

std::vector<Listening>* Webserv::getListenings()
{
	return listenings_;
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
			std::cerr << strerror(errno) << std::endl;
			exit(1);
		}
		int optval = 1;
		if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		{
			std::cerr << strerror(errno) << std::endl;
			exit(1);
		}
		/* nonblocking(); */
		if (bind(s, (struct sockaddr*)&it->sockaddrIn, it->socklen) != 0)
		{
			std::cerr << strerror(errno) << std::endl;
			exit(1);
		}
		if (listen(s, it->backlog) == -1)
		{
			std::cerr << strerror(errno) << std::endl;
			exit(1);
		}
		it->sfd = s;
	}
}

void Webserv::processLoop()
{
	for (;;)
	{
		ev->processEvents(*this);
	}
}
