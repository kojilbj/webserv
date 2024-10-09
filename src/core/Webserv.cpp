#include "Webserv.hpp"

using namespace Webserv;

void Webserv::init() { }

void Webserv::openListeningSocket(Webserv& ws)
{
	std::vector<Listening> lss = ws.listenings;
	std::vector<Listening>::iterator it;
	int s;

	for (it = lss.begin(); it != lss.end(); it++)
	{
		s = socket(it->sockaddr->sa_family, it->type, 0);
		/* setsockopt(); */
		/* nonblocking(); */
		bind(s, it->sockaddr, it->socklen);
		listen(s, it->backlog);
		it->fd = s;
	}
}

void Webserv::processCycle()
{
	for (;;)
	{
		processEventsAndTimers(*this);
	}
}
