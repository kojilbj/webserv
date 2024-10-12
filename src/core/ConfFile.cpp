#include "ConfFile.hpp"

using namespace Wbsv;

void HttpConfCtx::initListening(std::vector<Listening>* lss) const
{
	std::vector<ServerCtx>::const_iterator it;
	for (it = serverCtxs_.begin(); it != serverCtxs_.end(); it++)
	{
		Listening ls;
		ls.protocol = getProtocol();
		std::string host = (it->getListen()).first;
		std::string port = (it->getListen()).second;
		if (host == "unix")
		{
			/* handle unix domain socket */
		}
		else if (host[0] == '[')
		{
			/* handle inet_6 domain socket */
		}
		else
		{
			/* handle inet_4 domain socket */
			struct addrinfo hints;
			struct addrinfo* result;
			std::memset(&hints, 0, sizeof(struct addrinfo));
			/* you can use SOCK_DGRAM if add StreamConfCtx, but not HttpConfCtx */
			hints.ai_flags = AI_NUMERICSERV;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_family = AF_INET;
			hints.ai_canonname = NULL;
			hints.ai_addr = NULL;
			hints.ai_next = NULL;
			/* first argument should be nullptr ? */
			/* because all socket must be passive (AI_PASSIVE) */
			if (getaddrinfo(host.c_str(), port.c_str(), &hints, &result) != 0)
			{
				std::cerr << strerror(errno) << std::endl;
				exit(1);
			}
			ls.result = new struct addrinfo;
			ls.result->ai_flags = result->ai_flags;
			ls.result->ai_family = result->ai_family;
			ls.result->ai_socktype = result->ai_socktype;
			ls.result->ai_protocol = result->ai_protocol;
			ls.result->ai_addrlen = result->ai_addrlen;
			ls.result->ai_canonname = NULL;
			ls.result->ai_addr = new struct sockaddr;
			ls.result->ai_addr->sin_family = result->ai_addr->sin_family;
			ls.result->ai_addr->sin_port = result->ai_addr->sin_port;
			std::memcpy(
				&ls.result->ai_addr->sin_addr, &result->ai_addr->sin_addr, sizeof(struct in_addr));
			ls.result->ai_next = NULL;
			ls.backlog = 10;
			ls.protocol = "HTTP";
		}
		lss->push_back(ls);
	}
}
