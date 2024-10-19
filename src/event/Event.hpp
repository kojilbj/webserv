#ifndef EVENT_HPP
#define EVENT_HPP

#include "Protocol.hpp"
#include "Webserv.hpp"

namespace Wbsv
{
	class Protocol;
	class Webserv;

	class Event
	{
	public:
		virtual ~Event(){};
		virtual void init(Webserv& ws) = 0;
		virtual void processEventsLoop(Webserv& ws) = 0;
		virtual void processEvents(Webserv& ws) = 0;
		virtual void addEvent(int fd, Protocol* p, int option) = 0;
	};
} // namespace Wbsv

#endif
