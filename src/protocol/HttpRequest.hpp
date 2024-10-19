#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <cstring>
#include <string>
#include <vector>

namespace Wbsv
{
	class Connection;

	enum againFunctions
	{
		AGAIN_REQUESTLINE = 100,
		AGAIN_REQUESTHEADER
	};

	enum rule
	{
		SP = 32,
		CR = 13,
		LF = 10
	};

	enum requestLineState
	{
		METHOD = 0,
		SLASH_IN_URI,
		SEGMENT,
		URI,
		SPACE_BEFORE_VERSION,
		VERSION_H,
		VERSION_HT,
		VERSION_HTT,
		VERSION_HTTP,
		VERSION_SLASH,
		VERSION_MAJOR,
		VERSION_POINT,
		VERSION_MINOR,
		END_CR,
		END_LF,
		END
	};

	enum requestLineError
	{
		INVALID_METHOD,
		INVALID_URI,
		INVALID_VERSION
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
			, state(0){};
		int processRequestLine(Connection& c);
		int processRequestHeader(Connection& c);
		int parseRequestLine();
		int parseRequestHeaderLine();

	protected:
		std::string headerIn;
		/* default 1k byte */
		const unsigned int clientHeaderSize;
		/* default 8k byte */
		const unsigned int largeClientHeaderSize;

	private:
		std::string headerOut;
		unsigned int requestLineLen;
		int pos;
		int state;
		int method;
		std::string uri;
		int major;
		int minor;
	};
} // namespace Wbsv

#include "Connection.hpp"

#endif
