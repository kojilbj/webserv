#ifndef HTTP_HPP
#define HTTP_HPP

#include "Protocol.hpp"
#include <iostream>
#include <map>
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
		Http();
		~Http(){};

		/* Core handlers */
		revHandler_pt revHandler;
		void initPhaseHandler();
		int invokeRevHandler();
		void selectServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls);
		void selectVServerCtx(ServerCtx* serverCtx, string host);
		VServerCtx* getVServerCtx() const
		{
			return vserverCtx_;
		};
		LocationCtx* getLocationCtx() const
		{
			return locationCtx_;
		}
		void setLocationCtx(LocationCtx* locationCtx)
		{
			locationCtx_ = locationCtx;
		};
		std::string& getUri()
		{
			return uri;
		};
		int getMethod()
		{
			return method;
		};
		int getPos()
		{
			return pos;
		};
		void setFd(int fd)
		{
			fd_ = fd;
		}
		std::string& getHeaderIn()
		{
			return headerIn;
		};

		/* Request handlers */
		int waitRequestHandler();
		int processRequestLine();
		int processRequestHeader();
		int parseRequestLine();
		int parseRequestHeaderLine();
		int processRequest();

		/* Response handlers */
		int coreRunPhase();
		int finalizeRequest();

		/* Response variables */
		string statusLine;
		string headerOut;
		string messageBodyOut;
		map<string, string> defaultErrorPages;

	private:
		/* Request variables */
		string headerIn;
		map<string, string> headersIn;
		string headerFieldNameTmp;
		string headerFieldValueTmp;
		VServerCtx* vserverCtx_;
		LocationCtx* locationCtx_;
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
		bool alreadyWrite;
		bool ready;
		int responseState;
		// pipe or regular file fd to send to the client as message body
		int fd_;
	};
} // namespace Wbsv

#include "Access.hpp"
#include "ConfFile.hpp"
#include "Connection.hpp"
#include "Content.hpp"
#include "FindConfig.hpp"
#include "HttpConfFile.hpp"
#include "Listening.hpp"
#include "PhaseHandler.hpp"
#include "Rewrite.hpp"

#endif
