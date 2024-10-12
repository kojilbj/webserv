#ifndef EPOLLMODULE_HPP
#define EPOLLMODULE_HPP

#include "../../core/Webserv.hpp"
#include "Module.hpp"
#include <sys/epoll.h>

namespace Webserv
{
	class Epoll : public Module
	{
	public:
		void init(Webserv& ws);
		/* processEvents(Webserv& ws); */

	private:
		int ep;
		struct epoll_event eventList;
		/* int maxEvents; */
	};

} // namespace Webserv

#endif
