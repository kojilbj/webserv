#ifndef CONFFILE_HPP
#define CONFFILE_HPP

#include "Connection.hpp"
#include <string>
#include <vector>

namespace Wbsv
{
	class Webserv;

	class ConfCtx
	{
	public:
		virtual ~ConfCtx(){};
		virtual std::string getProtocol() const = 0;
		virtual void initListening(Listening&) const = 0;
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
		std::string server_name;
		const std::pair<std::string, std::string>& getListen() const
		{
			return listen_;
		}
		const std::vector<LocationCtx>& getLocationCtxs() const
		{
			return locationCtxs_;
		}
		void addListen(const std::string& host, const std::string& port)
		{
			listen_.first = host;
			listen_.second = port;
		}
		void addLocationCtx(const LocationCtx& lc)
		{
			locationCtxs_.push_back(lc);
		}

	private:
		std::pair<std::string, std::string> listen_;
		std::vector<LocationCtx> locationCtxs_;
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
		const std::vector<VServerCtx>& getVServerCtxs() const
		{
			return vserverCtxs_;
		}
		void addVServerCtx(const VServerCtx& vsc)
		{
			vserverCtxs_.push_back(vsc);
		}
		std::string getProtocol() const
		{
			return "HTTP";
		}
		void initListening(Listening& ls) const
		{
			ls.type = SOCK_STREAM;
			ls.
		}

	private:
		/* this variable size is always 0, but you can add some directive here */
		std::vector<std::vector<std::string> > mainCtxs_;
		std::vector<VServerCtx> vserverCtxs_;
	};

	void confParse(Webserv& ws);

} // namespace Wbsv

#endif
