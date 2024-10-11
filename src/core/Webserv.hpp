#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "ConfFile.hpp"
#include "Connection.hpp"
#include <vector>
/* #include <errno.h> */

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
