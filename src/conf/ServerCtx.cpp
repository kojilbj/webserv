#include "ServerCtx.hpp"

ServerCtx::ServerCtx(void)
{
#ifdef DEBUG
	std::cout << "ServerCtx Constructor Called" << std::endl;
#endif
	listenPort_ = 80;
	listenIP_ = "0.0.0.0";
}

ServerCtx::ServerCtx(const ServerCtx& other)
	: listenPort_(other.listenPort_), listenIP_(other.listenIP_)
{
	this->vServerCtxs_.clear();
	for (std::vector<VServerCtx>::const_iterator it = other.vServerCtxs_.begin();
		 it != other.vServerCtxs_.end();
		 it++)
	{
		this->vServerCtxs_.push_back(*it);
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

std::pair<std::string, std::string> ServerCtx::getListen(void) const
{
	std::pair<std::string, std::string> listen;

	listen.first = listenIP_;
	listen.second = listenPort_;
	return listen;
}

const std::vector<VServerCtx>& ServerCtx::getVServers(void) const
{
	return (vServerCtxs_);
}

void ServerCtx::addVServer(struct ConfParseUtil::SServer server)
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
	if (!server.defaultServer.empty())
		vsctx.setDefaultServer(server.defaultServer);
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
	vServerCtxs_.push_back(vsctx);
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
		this->vServerCtxs_.clear();
		for (std::vector<VServerCtx>::const_iterator it = other.vServerCtxs_.begin();
			 it != other.vServerCtxs_.end();
			 it++)
		{
			this->vServerCtxs_.push_back(*it);
		}
	}
	return *this;
}