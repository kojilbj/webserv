#ifndef HTTP_HPP
#define HTTP_HPP

#include "Protocol.hpp"
#include "log.hpp"
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
		GET = 1,
		POST = 2,
		DELETE = 4
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
		void setMethod(int m)
		{
			method = m;
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
		int parseChunkedRequest(const char* buf, size_t size);

		// Upstream handlers
		int processUpstream();
		int emptyHandler();
		void setChildPid(int pid)
		{
			childPid_ = pid;
		}
		int getChildPid()
		{
			return childPid_;
		}

		/* Response handlers */
		int createResponse(const std::string&);
		int coreRunPhase();
		int finalizeRequest();
		int readDiscardedRequest();

		/* Response variables */
		string statusLine;
		string headerOut;
		string messageBodyOut;

		// file
		int getRequestBodyFileFd()
		{
			return requestBodyFileFd_;
		}
		int getResponseBodyFileFd()
		{
			return responseBodyFileFd_;
		}
		void setRequestBodyFileFd(const int fd)
		{
			requestBodyFileFd_ = fd;
		}
		void setResponseBodyFileFd(const int fd)
		{
			responseBodyFileFd_ = fd;
		}
		void closeRequestBodyFileFd()
		{
			if (requestBodyFileFd_ == -1)
				throw std::string("Warning: you are going to close closed connection !!!\n");
			close(requestBodyFileFd_);
			requestBodyFileFd_ = -1;
		}
		void closeResponseBodyFileFd()
		{
			if (responseBodyFileFd_ == -1)
				throw std::string("Warning: you are going to close closed connection !!!\n");
			close(responseBodyFileFd_);
			responseBodyFileFd_ = -1;
		}
		std::string requestBodyFileName_;
		std::string responseBodyFileName_;

		// Error
		bool notFound;
		bool forbidden;
		bool internalRedirect;
		map<string, string> defaultErrorPages;
		bool completelyRead;

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
		size_t pos;
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
		bool continueRequest_;
		// regular file fd to send to the client as message body
		int fd_;
		int childPid_;
		int requestBodyFileFd_;
		int responseBodyFileFd_;
		int bodyLen_;
		int otherThanChunkDataSize_;
		size_t chunkSize_;
		std::string chunkSizeStr_;
		size_t countChunkData_;
		char chunkedRequestBuf_[1025];
		size_t unchunkedRequestSize_;
		char unchunkedRequestBuf_[1025];
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
