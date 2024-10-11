#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* #include "ConfFile.hpp" */
#include "Connection.hpp"
/* #include <errno.h> */
#include <iostream>
#include <vector>

/* substitution for test */

/* http { */
/* 	server { */
/* 		listen localhost:80; */
/* 		server_name localhost; */

/* 		location / { */
/* 			root /root/webserv/test/html; */
/* 			index index.html; */
/* 		} */
/* 	} */
/* } */

namespace Wbsv
{
	class Webserv;

	class ConfCtx
	{
	public:
		virtual ~ConfCtx(){};
		virtual std::string getProtocol() = 0;
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
		std::pair<std::string, std::string> listen;
		std::string server_name;
		std::vector<LocationCtx> locationCtxs;
	};

	class HttpConfCtx : public ConfCtx
	{
	public:
		HttpConfCtx(){};
		~HttpConfCtx(){};
		std::string getProtocol()
		{
			return "HTTP";
		};

		/* this variable size is always 0, but you can add some directive here */
		std::vector<std::vector<std::string>> mainCtxs_;
		std::vector<VServerCtx> vserverCtxs_;
	};

	void confParse(Webserv& ws);

} // namespace Wbsv
/* end */

namespace Wbsv
{
	class Webserv
	{
	public:
		Webserv();
		~Webserv();

		std::vector<ConfCtx*>* getConfCtxs();
		void setConfCtxs(std::vector<ConfCtx*>* confCtxs);
		void init();
		/* void openListeningSocket(); */
		/* void processLoop(); */

	private:
		std::vector<ConfCtx*>* confCtxs_;
		std::vector<Listening>* listenings_;
	};
} // namespace Wbsv

/* extern Epoll ev; */

#endif
