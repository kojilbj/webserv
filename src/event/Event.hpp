#ifndef EVENT_HPP
#define EVENT_HPP

#include "Protocol.hpp"
#include "Webserv.hpp"

namespace Wbsv
{
	class Protocol;
	class Webserv;

	typedef enum
	{
		ListeningFd = 0,
		ConnectionFd,
		UpstreamFd,
	} type_t;

	typedef union
	{
		Listening* ls;
		Protocol* p;
		Upstream* upstream;
	} data_t;

	struct eventData
	{
		int type;
		data_t data;
	};

	class Event
	{
	public:
		virtual ~Event(){};
		virtual void init(Webserv& ws) = 0;
		virtual void processEventsLoop(Webserv& ws) = 0;
		virtual void processEvents(Webserv& ws) = 0;
		virtual void addEvent(int fd, data_t& data, int type, int option) = 0;
		std::list<struct eventData*> freeList;
	};
} // namespace Wbsv

#endif
