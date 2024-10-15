#ifndef EPOLLEVNET_HPP
#	define EPOLLEVENT_HPP

#	include "Connection.hpp"
#	include "Http.hpp"
#	include "Event.hpp"
#	include "Webserv.hpp"
#	include <sys/epoll.h>
#	include <sys/socket.h>
#	include <unistd.h>
#	include <fcntl.h>

namespace Wbsv
{
	class Epoll : public Event
	{
	public:
		void init(Webserv& ws);
		void processEvents(Webserv& ws);

	private:
		int ep;
		struct epoll_event eventList;
	};

} // namespace Wbsv

#endif
