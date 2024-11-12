#include "HttpConfCtx.hpp"

using namespace Wbsv;

void HttpConfCtx::initListening(std::vector<Listening>* lss) const
{
	std::vector<ServerCtx>::const_iterator it;
	for (it = serverCtxs_.begin(); it != serverCtxs_.end(); it++)
	{
		Listening ls;
		ls.protocol = name;
		std::string host = (it->getListen()).first;
		std::string port = (it->getListen()).second;
		if (host == "unix")
		{
			/* add unix domain socket code if you want */
		}
		else if (host[0] == '[')
		{
			/* add inet_6 domain socket code if you want */
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
				std::cerr << "getaddrinfo: " << strerror(errno) << std::endl;
				exit(1);
			}
			ls.family = result->ai_family;
			ls.socktype = result->ai_socktype;
			ls.socklen = result->ai_addrlen;
			std::memcpy(&ls.sockaddrIn, result->ai_addr, sizeof(struct sockaddr_in));
			ls.backlog = 10;
			freeaddrinfo(result);
		}
		lss->push_back(ls);
	}
}


ServerCtx* HttpConfCtx::getServerCtx(const std::string& ipAddress, const std::string& port)
{
	for (std::vector<ServerCtx>::iterator it = serverCtxs_.begin(); it != serverCtxs_.end(); it++)
	{
		if ((*it).getIpAddress() == ipAddress && (*it).getPort() == port)
			return &(*it);
	}
	return NULL;
}

void HttpConfCtx::addServer(struct ConfParseUtil::SServer serverInfo)
{
	if (serverInfo.listenIP.empty())
		serverInfo.listenIP = "0.0.0.0";
	if (serverInfo.listenIP == "localhost")
		serverInfo.listenIP = "127.0.0.1";
	if (serverInfo.listenPort.empty())
		serverInfo.listenPort = "80";
	for (std::vector<ServerCtx>::iterator it = serverCtxs_.begin(); it != serverCtxs_.end(); it++)
	{
		if (it->getIpAddress() == serverInfo.listenIP && it->getPort() == serverInfo.listenPort)
		{
			(*it).addVServer(serverInfo);
			return;
		}
	}
	addServerCtx();
	ServerCtx& serverCtx = serverCtxs_.back();
	if (!serverInfo.listenIP.empty())
		serverCtx.setListenIP(serverInfo.listenIP);
	if (!serverInfo.listenPort.empty())
		serverCtx.setListenPort(serverInfo.listenPort);
	serverCtx.addVServer(serverInfo);
}