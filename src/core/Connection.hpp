#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "../protocol/Protocol.hpp"
#include "Webserv.hpp"
#include <cstring>
#include <netdb.h>
#include <string>
#include <sys/socket.h>

namespace Wbsv //Core
{
	class Listening
	{
	public:
		Listening();
		~Listening();
		Listening(const Listening& other);
		int sfd;
		int family;
		int socktype;
		struct sockaddr_in sockaddrIn;
		socklen_t socklen;
		int backlog;
		/* Http */
		std::string protocol;
	};

	class Connection
	{
	public:
		Connection(){};
		~Connection(){};
		void setListening(Listening* listening);
		void setAcceptRev(int fd, struct sockaddr_in* sockaddrIn, socklen_t socklen);

		int cfd;
		struct sockaddr_in remoteSockaddrIn;
		socklen_t remoteSocklen;
		Listening* ls;
	};
} // namespace Wbsv

#endif
