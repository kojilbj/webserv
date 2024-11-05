#ifndef LISTENING_HPP
#define LISTENING_HPP

#include <cstring>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

namespace Wbsv
{
	class Listening
	{
	public:
		Listening();
		~Listening();
		Listening(const Listening& other);
		Listening& operator=(const Listening& other);
		int sfd;
		int family;
		int socktype;
		struct sockaddr_in sockaddrIn;
		socklen_t socklen;
		int backlog;
		/* Http */
		std::string protocol;
		const std::string type;
	};

} // namespace Wbsv

#endif
