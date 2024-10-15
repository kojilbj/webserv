#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "ConfFile.hpp"
#include "Event.hpp"
#include "Listening.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <vector>

namespace Wbsv
{
	class ConfCtx;
	class Listening;
	class Epoll;

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
		void processLoop();

	private:
		std::vector<ConfCtx*>* confCtxs_;
		std::vector<Listening>* listenings_;
	};

} // namespace Wbsv

#endif
