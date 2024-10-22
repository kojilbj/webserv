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
		Http(){};
		~Http(){};
		void initPhaseHandler();
		int invokeRevHandler(Connection& c);
		void setRevHandler(revHandler_pt func);
		void getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls);
		int waitRequestHandler(Connection& c);
		void processRequest();
		void coreRunPhase();
		void finalizeRequest();

	private:
		LocationCtx* locationCtx;
	};

} // namespace Wbsv

#include "ConfFile.hpp"
#include "Connection.hpp"
#include "FindConfig.hpp"
#include "HttpConfFile.hpp"
#include "Listening.hpp"
#include "PhaseHandler.hpp"

#endif
