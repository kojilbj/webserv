#include "HttpConfCtx.hpp"

HttpConfCtx::HttpConfCtx(void)
{
#ifdef DEBUG
	std::cout << "HttpConfCtx Constructor Called" << std::endl;
#endif
	name = "http";
}

HttpConfCtx::~HttpConfCtx(void) { }

void HttpConfCtx::addServerCtx(ServerCtx serverCtx)
{
	servers_.push_back(serverCtx);
}

void HttpConfCtx::addServerCtx(void)
{
	servers_.push_back(ServerCtx());
}

std::vector<ServerCtx>& HttpConfCtx::getServerCtxs(void)
{
	return (servers_);
}

ServerCtx* HttpConfCtx::getServerCtx(const std::string& ipAddress, const std::string& port)
{
	for (std::vector<ServerCtx>::iterator it = servers_.begin(); it != servers_.end(); it++)
	{
		if ((*it).getIpAddress() == ipAddress && (*it).getPort() == port)
			return &(*it);
	}
	return nullptr;
}

void HttpConfCtx::addServer(struct ConfParseUtil::SServer serverInfo)
{
	if (serverInfo.listenIP.empty())
		serverInfo.listenIP = "0.0.0.0";
	if (serverInfo.listenIP == "localhost")
		serverInfo.listenIP = "127.0.0.1";
	if (serverInfo.listenPort.empty())
		serverInfo.listenPort = "80";
	for (std::vector<ServerCtx>::iterator it = servers_.begin(); it != servers_.end(); it++)
	{
		if (it->getIpAddress() == serverInfo.listenIP && it->getPort() == serverInfo.listenPort)
		{
			(*it).addVServer(serverInfo);
			return;
		}
	}
	addServerCtx();
	ServerCtx& serverCtx = servers_.back();
	if (!serverInfo.listenIP.empty())
		serverCtx.setListenIP(serverInfo.listenIP);
	if (!serverInfo.listenPort.empty())
		serverCtx.setListenPort(serverInfo.listenPort);
	serverCtx.addVServer(serverInfo);
}