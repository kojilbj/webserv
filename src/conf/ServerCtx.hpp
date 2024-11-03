#ifndef SERVERCTX_HPP
#define SERVERCTX_HPP

#include "VServerCtx.hpp"
#include <vector>

namespace Wbsv
{
	class ServerCtx
	{
	public:
		const std::pair<std::string, std::string>& getListen() const
		{
			return listen_;
		}
		std::vector<VServerCtx>* getVServerCtxs()
		{
			return &vserverCtxs_;
		}
		void addListen(const std::string& host, const std::string& port)
		{
			listen_.first = host;
			listen_.second = port;
		}
		void addVServerCtx(const VServerCtx& vsc)
		{
			vserverCtxs_.push_back(vsc);
		}

	private:
		std::pair<std::string, std::string> listen_;
		std::vector<VServerCtx> vserverCtxs_;
	};
} // namespace Wbsv

#endif