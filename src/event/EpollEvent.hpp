#ifndef EPOLLEVNET_HPP
#	define EPOLLEVENT_HPP

#	include "Connection.hpp"
#	include "log.hpp"
#	include "Listening.hpp"
#	include "Protocol.hpp"
#	include "Http.hpp"
#	include "Event.hpp"
#	include "Webserv.hpp"
#	include <sys/epoll.h>
#	include <sys/socket.h>
#	include <unistd.h>
#	include <fcntl.h>
#	include <list>

namespace Wbsv
{
	class Epoll : public Event
	{
	public:
		Epoll();
		~Epoll();
		void init(Webserv& ws);
		void processEventsLoop(Webserv& ws);
		void processEvents(Webserv& ws);
		void addEvent(int fd, data_t& data, int type, int option);
		void timeOutHandler(Webserv& ws);

	private:
		int ep;
		std::list<struct eventData*> freeList;
	};

} // namespace Wbsv

#endif
