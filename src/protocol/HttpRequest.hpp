#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <cstring>
#include <vector>

namespace Wbsv
{
	class Connection;

	enum againFunctions
	{
		AGAIN_REQUESTLINE = 0,
		AGAIN_REQUESTHEADER
	};

	class HttpRequest
	{
	public:
		HttpRequest()
			: clientHeaderSize(1024), largeClientHeaderSize(8192), requestLineLen(0){};
		int processRequestLine(Connection& c);
		int processRequestHeader(Connection& c);
		int parseRequestLine();
		int parseRequestHeaderLine();

	protected:
		std::vector<char> headerIn;
		/* default 1k byte */
		const unsigned int clientHeaderSize;
		/* default 8k byte */
		const unsigned int largeClientHeaderSize;

	private:
		std::vector<char> headerOut;
		unsigned int requestLineLen;
		std::vector<char>::iterator pos;
	};
} // namespace Wbsv

#include "Connection.hpp"

#endif
