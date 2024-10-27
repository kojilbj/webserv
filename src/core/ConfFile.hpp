#ifndef CONFFILE_HPP
#define CONFFILE_HPP

#include "Connection.hpp"
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
	class Webserv;
	void errorExit(int errnum);

	class ConfCtx
	{
	public:
		virtual ~ConfCtx(){};
		virtual std::string getProtocol() const = 0;
		virtual void initListening(std::vector<Listening>*) const = 0;
	};

	class LocationCtx
	{
	public:
		std::string path;
		std::string root;
		std::string index;
	};

	class VServerCtx
	{
	public:
		bool defaultServer;
		std::string serverName;

		VServerCtx()
			: defaultServer(false){};
		~VServerCtx(){};
		const std::vector<LocationCtx>& getLocationCtxs() const
		{
			return locationCtxs_;
		}
		void addLocationCtx(const LocationCtx& lc)
		{
			locationCtxs_.push_back(lc);
		}

	private:
		std::vector<LocationCtx> locationCtxs_;
	};

	class ServerCtx
	{
	public:
		const std::pair<std::string, std::string>& getListen() const
		{
			return listen_;
		}
		const std::vector<VServerCtx>& getVServerCtxs() const
		{
			return vserverCtxs_;
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

	class HttpConfCtx : public ConfCtx
	{
	public:
		HttpConfCtx(){};
		~HttpConfCtx(){};
		const std::vector<std::vector<std::string> >& getMainCtxs() const
		{
			return mainCtxs_;
		}
		const std::vector<ServerCtx>& getServerCtxs() const
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

	void confParse(Webserv& ws);

} // namespace Wbsv

#endif
