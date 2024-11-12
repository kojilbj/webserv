#ifndef HTTPCONFCTX_HPP
#define HTTPCONFCTX_HPP

#include <iostream>
#include <vector>

#include "ConfCtx.hpp"
#include "ConfParseUtil.hpp"
#include "ServerCtx.hpp"

class HttpConfCtx : public ConfCtx
{
public:
	HttpConfCtx(void);
	~HttpConfCtx(void);

	// getMainCtx(void);
	// getServerCtx(void);
	// void addServerCtx(ServerCtx serverCtx);
	void addServer(struct ConfParseUtil::SServer server);
	std::vector<ServerCtx>& getServerCtxs(void);
	ServerCtx* getServerCtx(const std::string& ipAddress, const std::string& port);
	// initListening(void);

private:
	std::vector<ServerCtx> servers_;
	void addServerCtx(void);
	void addServerCtx(ServerCtx serverCtx);
};

#endif
