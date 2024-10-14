#ifndef EVENT_HPP
#define EVENT_HPP

#include "Webserv.hpp"

namespace Wbsv
{
	class Webserv;

	class Event
	{
	public:
		virtual ~Event(){};
		virtual void init(Webserv& ws) = 0;
		virtual void processEvents(Webserv& ws) = 0;
	};
} // namespace Wbsv

#endif
