#include "Event.hpp"

using namespace Webserv;

void proceeEventsAndTimers(Webserv& ws)
{
	ev.processEvents(ws);
}
