#include "Event.hpp"

using namespace Webserv;

extern Epoll ev;

void proceeEventsAndTimers(Webserv& ws)
{
	ev.processEvents(ws);
}
