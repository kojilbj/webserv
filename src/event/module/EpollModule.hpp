#ifndef EPOLLMODULE_HPP
#define EPOLLMODULE_HPP

#include "Module.hpp"
#include "Webserv.hpp"
#include <sys/epoll.h>

namespace Webserv
{
	class Epoll : public Module
	{
	public:
		init(Webserv::Webserv& ws);
		processEvents(Webserv::Webserv& ws);

	private:
		int ep;
		struct epoll_event* eventList;
		int nevents;
	}

} // namespace Webserv

#endif
