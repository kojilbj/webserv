#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "ConfFile.hpp"
#include "Connection.hpp"
#include <vector>

namespace Webserv
{
	class Webserv
	{
	public:
		Webserv();
		~Webserv();
		void init();
		void openListeningSocket();
		void processCycle();

	private:
		std::vector<ConfCtx> confCtxs;
		std::vector<Listening> listenings;
		std::vector<Connection> connections;
	}
} // namespace Webserv

#endif
