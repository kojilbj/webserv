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
			if (getaddrinfo(host.c_str(), port.c_str(), &hints, &ls.result) != 0)
			{
				std::cerr << strerror(errno) << std::endl;
				exit(1);
			}
		}
		lss->push_back(ls);
	}
}
