#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <cstring>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;

namespace Wbsv
{
	class Connection;
	class ServerCtx;
	class VServerCtx;

	enum rule
	{
		SP = 32,
		CR = 13,
		LF = 10
	};

	enum httpMethod
	{
		GET,
		POST,
		DELETE
	};

	class HttpRequest
	{
	public:
		HttpRequest()
			: clientHeaderSize(1024)
			, largeClientHeaderSize(8192)
			, requestLineLen(0)
			, pos(0)
			, start(0)
			, state(0){};
		int processRequestLine(Connection& c);
		int processRequestHeader(Connection& c);
		int parseRequestLine();
		int parseRequestHeaderLine();
		void getVServerCtx(ServerCtx* serverCtx, string host);

		/* protected: */
		string headerIn;
		map<string, string> headersIn;
		/* default 1k byte */
		const unsigned int clientHeaderSize;
		/* default 8k byte */
		const unsigned int largeClientHeaderSize;
		VServerCtx* vserverCtx;

		/* private: */
		string headerOut;
		unsigned int requestLineLen;
		int pos;
		int start;
		int state;
		int method;
		string uri;
		int major;
		int minor;

		string headerFieldNameTmp;
		string headerFieldValueTmp;
	};
} // namespace Wbsv

#include "ConfFile.hpp"
#include "Connection.hpp"

#endif
