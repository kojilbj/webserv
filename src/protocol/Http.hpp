#ifndef HTTP_HPP
#define HTTP_HPP

#include "HttpRequest.hpp"
#include "Protocol.hpp"
#include <iostream>
#include <unistd.h>

namespace Wbsv
{
	class HttpConfCtx;
	class HttpRequest;
	class ServerCtx;
	class Protocol;

	class Http : public Protocol, public HttpRequest
	{
	public:
		Http()
			: serverCtx(NULL){};
		~Http(){};
		void getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls);
		int waitRequestHandler(Connection& c);

	private:
		ServerCtx* serverCtx;
		void wevHandler();
	};

} // namespace Wbsv

#include "ConfFile.hpp"
#include "Connection.hpp"
#include "HttpConfFile.hpp"
#include "Listening.hpp"

#endif
