#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <cstring>
#include <ctime>
#include <netdb.h>
#include <sys/socket.h>

namespace Wbsv
{
	class Listening;
	class ServerCtx;

	class Connection
	{
	public:
		Connection();
		~Connection();
		void setAcceptRev(int fd, struct sockaddr_in* sockaddrIn, socklen_t socklen);

		int cfd;
		struct sockaddr_in remoteSockaddrIn;
		socklen_t remoteSocklen;
		Listening* ls;
		ServerCtx* serverCtx;
		std::time_t lastReadTime;
	};
} // namespace Wbsv

#endif
