#ifndef HTTP_HPP
#define HTTP_HPP

#include "HttpRequest.hpp"
#include "Protocol.hpp"
#include <iostream>
#include <unistd.h>

using std::map;
using std::string;

namespace Wbsv
{
	class HttpConfCtx;
	class ServerCtx;
	class Protocol;
	class Connection;
	class VServerCtx;

	typedef int (Http::*revHandler_pt)();

	enum rule
	{
		SP = 32,
		CR = 13,
		LF = 10
	};

	enum httpMethod
	{
		GET = 0,
		POST,
		DELETE
	};

	class Http : public Protocol
	{
	public:
		Http()
			: clientHeaderSize(1024)
			, largeClientHeaderSize(8192)
			, requestLineLen(0)
			, pos(0)
			, start(0)
			, state(0)
			, method(0)
			, major(0)
			, minor()
			, alreadyRead(false)
			, ready(false){};
		~Http(){};

		/* Core handlers */
		revHandler_pt revHandler;
		void initPhaseHandler();
		int invokeRevHandler();
		void getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls);

		/* Request handlers */
		int waitRequestHandler();
		int processRequestLine();
		int processRequestHeader();
		int parseRequestLine();
		int parseRequestHeaderLine();
		int processRequest();
		void getVServerCtx(ServerCtx* serverCtx, string host);

		/* Response handlers */
		int coreRunPhase();
		int finalizeRequest();

	private:
		/* Request variables */
		string headerIn;
		map<string, string> headersIn;
		string headerFieldNameTmp;
		string headerFieldValueTmp;
		VServerCtx* vserverCtx;
		LocationCtx* locationCtx;
		/* default 1k byte */
		const unsigned int clientHeaderSize;
		/* default 8k byte */
		const unsigned int largeClientHeaderSize;
		unsigned int requestLineLen;
		int pos;
		int start;
		int state;
		int method;
		string uri;
		int major;
		int minor;
		bool alreadyRead;
		bool ready;

		/* Response variables */
		string headerOut;
	};

} // namespace Wbsv

#include "ConfFile.hpp"
#include "Connection.hpp"
#include "FindConfig.hpp"
#include "HttpConfFile.hpp"
#include "Listening.hpp"
#include "PhaseHandler.hpp"

#endif
