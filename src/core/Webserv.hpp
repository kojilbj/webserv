#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "../event/module/EpollModule.hpp"
#include "ConfFile.hpp"
#include "Connection.hpp"
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
