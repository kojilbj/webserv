#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <list>
#include <signal.h>
#include <sys/socket.h>
#include <vector>

namespace Wbsv
{
	class ConfCtx;
	class Protocol;
	class Listening;
	class Epoll;

	enum returnVal
	{
		OK = 0,
		ERROR,
		DONE,
		AGAIN
	};

	enum eventCtl
	{
		ADD = 0,
		MOD
	};

	class Webserv
	{
	public:
		Webserv();
		~Webserv();

		std::vector<ConfCtx*>* getConfCtxs();
		std::vector<Listening>* getListenings();
		void setConfCtxs(std::vector<ConfCtx*>* confCtxs);
		void init();
		void openListeningSocket();
		void acceptEvent(Listening* ls);

	private:
		std::vector<ConfCtx*>* confCtxs_;
		std::vector<Listening>* listenings_;
		std::list<Protocol*> freeList;
	};

} // namespace Wbsv

#include "ConfFile.hpp"
#include "Event.hpp"
#include "Http.hpp"
#include "Listening.hpp"
#include "Protocol.hpp"

#endif
