#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Listening.hpp"
#include <cstring>
#include <sys/socket.h>

namespace Wbsv
{
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
