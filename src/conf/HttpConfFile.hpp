#ifndef HTTPCONFFILE_HPP
#define HTTPCONFFILE_HPP

#include "ConfFile.hpp"
#include "Listening.hpp"
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

	class HttpConfCtx : public ConfCtx
	{
	public:
		HttpConfCtx(){};
		~HttpConfCtx(){};
		const std::vector<std::vector<std::string> >& getMainCtxs() const
		{
			return mainCtxs_;
		}
		std::vector<ServerCtx>& getServerCtxs()
		{
			return serverCtxs_;
		}
		void addServerCtx(const ServerCtx& sc)
		{
			serverCtxs_.push_back(sc);
		}
		std::string getProtocol() const
		{
			return "HTTP";
		}
		void initListening(std::vector<Listening>* lss) const;

	private:
		/* this variable size is always 0, but you can add some directive here */
		std::vector<std::vector<std::string> > mainCtxs_;
		std::vector<ServerCtx> serverCtxs_;
	};
} // namespace Wbsv

#endif
