#include "ServerCtx.hpp"

using namespace Wbsv;

ServerCtx::ServerCtx(void)
{
#ifdef DEBUG
	std::cout << "ServerCtx Constructor Called" << std::endl;
#endif
	listenIP_ = "0.0.0.0";
	listenPort_ = 80;
}

ServerCtx::ServerCtx(const ServerCtx& other)
	: listenIP_(other.listenIP_), listenPort_(other.listenPort_)
{
	this->vserverCtxs_.clear();
	for (std::vector<VServerCtx>::const_iterator it = other.vserverCtxs_.begin();
		 it != other.vserverCtxs_.end();
		 it++)
	{
		this->vserverCtxs_.push_back(*it);
	}
}

const std::string& ServerCtx::getIpAddress(void) const
{
	return (listenIP_);
}

const std::string& ServerCtx::getPort(void) const
{
	return (listenPort_);
}

const std::vector<VServerCtx>& ServerCtx::getVServers(void) const
{
	return (vserverCtxs_);
}

void ServerCtx::addVServer(struct ConfParseUtil::SServer server, bool isFirst)
{
	VServerCtx vsctx;

	if (!server.locations.empty())
		vsctx.addLocation(server.locations);
	if (!server.serverNames.empty())
	{
		for (std::vector<std::string>::iterator it = server.serverNames.begin();
			 it != server.serverNames.end();
			 it++)
		{
			if (it == server.serverNames.begin())
				vsctx.setServerName(*it);
			else
				vsctx.addServerName(*it);
		}
	}
	if (!server.clientMaxBodySize.empty())
		vsctx.setClientMaxBodySize(server.clientMaxBodySize);
	for (std::vector<ErrorPage>::const_iterator it = server.errorPages.getErrorPages().begin();
		 it != server.errorPages.getErrorPages().end();
		 it++)
	{
		for (std::vector<std::string>::const_iterator ite = (*it).getErrorNumber().begin();
			 ite != (*it).getErrorNumber().end();
			 ite++)
		{
			vsctx.addErrorPage(*ite, (*it).getErrorPagePath());
		}
	}
	vsctx.setDefaultServer(isFirst);
	vserverCtxs_.push_back(vsctx);
}

void ServerCtx::setListenPort(const std::string& listenPort)
{
	if (ConfParseUtil::isValidPort(listenPort) == false)
		throw std::logic_error("Error Invalid Port: " + listenPort);
	listenPort_ = listenPort;
}

void ServerCtx::setListenIP(const std::string& listenIP)
{
	if (ConfParseUtil::isValidIPAddress(listenIP) == false)
		throw std::logic_error("Error Invalid IP: " + listenIP);
	if (listenIP == "localhost")
	{
		listenIP_ = "127.0.0.1";
		return;
	}
	listenIP_ = listenIP;
}

ServerCtx& ServerCtx::operator=(const ServerCtx& other)
{
	if (&other != this)
	{
		listenPort_ = other.listenPort_;
		listenIP_ = other.listenIP_;
		this->vserverCtxs_.clear();
		for (std::vector<VServerCtx>::const_iterator it = other.vserverCtxs_.begin();
			 it != other.vserverCtxs_.end();
			 it++)
		{
			this->vserverCtxs_.push_back(*it);
		}
	}
	return *this;
}