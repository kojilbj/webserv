#ifndef SERVERCTX_HPP
#define SERVERCTX_HPP

#include <iostream>
#include <string>
#include <vector>

#include "ConfCtx.hpp"
#include "ConfParseUtil.hpp"
#include "LocationCtx.hpp"
#include "VServerCtx.hpp"

class ServerCtx
{
public:
	ServerCtx(void);
	ServerCtx(const ServerCtx& other);
	~ServerCtx(void);

	void setListenPort(const std::string& listenPort);
	void setListenIP(const std::string& listenIP);

	const std::string& getIpAddress(void) const;
	const std::string& getPort(void) const;
	const std::vector<VServerCtx>& getVServers(void) const;

	std::pair<std::string, std::string> getListen(void) const;

	void addVServer(struct ConfParseUtil::SServer server);

	ServerCtx& operator=(const ServerCtx& other);

private:
	std::string listenPort_;
	std::string listenIP_;
	std::vector<VServerCtx> vServerCtxs_;
};

#endif