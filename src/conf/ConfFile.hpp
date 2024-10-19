#ifndef CONFFILE_HPP
#define CONFFILE_HPP

#include <errno.h>
#include <string>
#include <vector>

namespace Wbsv
{
	class Webserv;
	class Listening;

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
		std::string server_name;

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

	class ConfCtx
	{
	public:
		virtual ~ConfCtx(){};
		virtual std::string getProtocol() const = 0;
		virtual void initListening(std::vector<Listening>*) const = 0;
	};

	void confParse(Webserv& ws);
} // namespace Wbsv

#include "Listening.hpp"
#include "Webserv.hpp"

#endif
