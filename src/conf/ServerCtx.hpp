#ifndef SERVERCTX_HPP
#define SERVERCTX_HPP

#include "ConfCtx.hpp"
#include "ConfParseUtil.hpp"
#include "LocationCtx.hpp"
#include "VServerCtx.hpp"
#include <vector>

namespace Wbsv
{
	class ServerCtx
	{
	public:
		ServerCtx(void);
		ServerCtx(const ServerCtx& other);
		~ServerCtx(void) { };

		void setListenPort(const std::string& listenPort);
		void setListenIP(const std::string& listenIP);

		const std::string& getIpAddress(void) const;
		const std::string& getPort(void) const;
		const std::vector<VServerCtx>& getVServers(void) const;

		ServerCtx& operator=(const ServerCtx& other);
		void addVServer(struct ConfParseUtil::SServer server, bool isFirst = false);

		std::pair<std::string, std::string> getListen() const
		{
			return make_pair(listenIP_, listenPort_);
		}
		std::vector<VServerCtx>* getVServerCtxs()
		{
			return &vserverCtxs_;
		}
		void addListen(const std::string& host, const std::string& port)
		{
			listenIP_ = host;
			listenPort_ = port;
		}
		void addVServerCtx(const VServerCtx& vsc)
		{
			vserverCtxs_.push_back(vsc);
		}

	private:
		std::string listenIP_;
		std::string listenPort_;
		std::vector<VServerCtx> vserverCtxs_;
	};
} // namespace Wbsv

#endif