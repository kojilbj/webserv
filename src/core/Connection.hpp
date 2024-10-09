#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Webserv.hpp"
#include <sys/socket.h>

namespace Webserv
{
	class Listening
	{
	public:
	private:
		int fd;
		int type;
		struct sockaddr_t sockaddr;
		socklen_t socklen;
		int backlog;
	}
} // namespace Webserv

#endif
