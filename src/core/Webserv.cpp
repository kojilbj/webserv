#include "Webserv.hpp"

using namespace Wbsv;

Webserv::Webserv()
	: confCtxs_(NULL) //, listenings_(NULL)
{ }

Webserv::~Webserv()
{
	std::vector<ConfCtx*>::iterator it;
	for (it = (*confCtxs_).begin(); it != (*confCtxs_).end(); it++)
	{
		delete *it;
	}
	delete confCtxs_;
}

std::vector<ConfCtx*>* Webserv::getConfCtxs()
{
	return confCtxs_;
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
		Listening ls;
		(*it)->initListening(ls);
		if ((*it)->getProtocol() == "HTTP")
		{
			HttpConfCtx* hcc = (HttpConfCtx*)*it;
			Listening ls;

			ls.type = SOCK_STREAM;
			ls.protocol = hcc->getProtocol();
		}
	}
}

/* void Webserv::openListeningSocket(Webserv& ws) */
/* { */
/* 	std::vector<Listening> lss = ws.listenings; */
/* 	std::vector<Listening>::iterator it; */
/* 	int s; */

/* 	for (it = lss.begin(); it != lss.end(); it++) */
/* 	{ */
/* 		s = socket(it->sockaddr->sa_family, it->type, 0); */
/* 		/1* setsockopt(); *1/ */
/* 		/1* nonblocking(); *1/ */
/* 		bind(s, it->sockaddr, it->socklen); */
/* 		listen(s, it->backlog); */
/* 		it->fd = s; */
/* 	} */
/* } */

/* void Webserv::processLoop() */
/* { */
/* 	for (;;) */
/* 	{ */
/* 		processEventsAndTimers(*this); */
/* 	} */
/* } */
