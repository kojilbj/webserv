#ifndef HTTP_HPP
#define HTTP_HPP

#include "Protocol.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <sys/stat.h>
#include <unistd.h>

using std::map;
using std::string;

namespace Wbsv
{
	class HttpConfCtx;
	class LocationCtx;
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
		void setUri(std::string& str)
		{
			uri = str;
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
		std::map<std::string, std::string>& getHeadersIn()
		{
			return headersIn;
		};
		std::string& getRequestBodyFileName()
		{
			return requestBodyFileName_;
		}
		void setUpstreamFd(int fd)
		{
			c.upstreamFd = fd;
		}

		/* Request handlers */
		int waitRequestHandler();
		int processRequestLine();
		int processRequestHeader();
		int parseRequestLine();
		int parseRequestHeaderLine();
		int processRequest();

		// Upstream handlers
		int processUpstream();
		int emptyHandler();

		/* Response handlers */
		int coreRunPhase();
		int finalizeRequest();

		/* Response variables */
		string statusLine;
		string headerOut;
		string messageBodyOut;
		std::string requestBodyFileName_;
		std::string responseBodyFileName_;

		// Error pages
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
		// regular file fd to send to the client as message body
		int fd_;
		int requestBodyFileFd_;
		int responseBodyFileFd_;
		int bodyLen_;
		char responseBodyBuf_[1024];
	};
} // namespace Wbsv

#include "Access.hpp"
#include "ConfCtx.hpp"
#include "Connection.hpp"
#include "Content.hpp"
#include "FindConfig.hpp"
#include "HttpConfCtx.hpp"
#include "Listening.hpp"
#include "LocationCtx.hpp"
#include "PhaseHandler.hpp"
#include "Rewrite.hpp"

#endif
