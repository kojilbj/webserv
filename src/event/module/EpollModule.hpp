#ifndef EPOLLMODULE_HPP
#define EPOLLMODULE_HPP

#include "../../core/Webserv.hpp"
#include "Module.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

namespace Wbsv //Event
{
	class Epoll : public Module
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
