#ifndef HTTPCONFCTX_HPP
#define HTTPCONFCTX_HPP

#include "ConfCtx.hpp"
#include "ConfParseUtil.hpp"
#include "ServerCtx.hpp"
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <vector>

namespace Wbsv
{
	class ConfCtx;
	class ServerCtx;

	class HttpConfCtx : public ConfCtx
	{
	public:
		HttpConfCtx()
			: ConfCtx("HTTP"){};
		~HttpConfCtx(){};

		void addServer(struct ConfParseUtil::SServer server);
		ServerCtx* getServerCtx(const std::string& ipAddress, const std::string& port);

		void addServerCtx(void)
		{
			serverCtxs_.push_back(ServerCtx());
		}
		void addServerCtx(const ServerCtx& sc)
		{
			serverCtxs_.push_back(sc);
		}
		const std::vector<std::vector<std::string> >& getMainCtxs() const
		{
			return mainCtxs_;
		}
		std::vector<ServerCtx>& getServerCtxs()
		{
			return serverCtxs_;
		}
		void initListening(std::vector<Listening>* lss) const;

	private:
		/* this variable size is always 0, but you can add some directive here */
		std::vector<std::vector<std::string> > mainCtxs_;
		std::vector<ServerCtx> serverCtxs_;
	};
} // namespace Wbsv

#include "Listening.hpp"
#include "ServerCtx.hpp"

#endif