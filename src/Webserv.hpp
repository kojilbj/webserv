#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "ConfFile.hpp"
#include "Event.hpp"
#include "Http.hpp"
#include "Listening.hpp"
#include "Protocol.hpp"
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
	class Listening;
	class Epoll;

	enum returnVal
	{
		OK = 0,
		ERROR,
		AGAIN
	};

	enum eventCtl
	{
		ADD = 0,
		MOD
	}

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

#endif
