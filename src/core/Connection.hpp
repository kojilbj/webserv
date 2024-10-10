#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Webserv.hpp"
#include <string>
#include <sys/socket.h>

namespace Webserv
{
	class Listening
	{
	public:
	private:
		int sfd;
		int type;
		struct sockaddr_t localSockaddr;
		socklen_t localSocklen;
		int backlog;
		std::string protocol;
	}

	class Connection
	{
	public:
		Connection();
		~Connection();
		void setListening(Listening* ls);
		void setAcceptRev(int cfd, struct sockaddr_t* sockaddr, socklen_t socklen);
		void (*revHandler)(Connection& c);
		void (*wevHandler)(Connection& c);

	private:
		int cfd;
		struct sockaddr_t* remoteSockaddr;
		socklen_t remoteSocklen;
		Listening* ls;
	}
} // namespace Webserv

#endif
