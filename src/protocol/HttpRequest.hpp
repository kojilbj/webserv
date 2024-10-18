#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "Connection.hpp"
#include "Http.hpp"
#include <cstring>
#include <vector>

namespace Wbsv
{
	class HttpRequest
	{
	public:
		void processRequestLine(Connection& c);
		void processRequestHeader(Connection& c);
		void parseRequestLine();
		void parseRequestHeader();

	private:
		std::vector<char> headerIn;
		std::vector<char> headerOut;
		int requestLineLen;
		/* default 1k byte */
		static const int clientHeaderSize;
		/* default 8k byte */
		static const int largeHeaderSize;
		std::vector<char>::iterator pos;
	};

	/* default 1k byte */
	const int clientHeaderSize = 1024;
	/* default 8k byte */
	const int largeHeaderSize = 8192;
};
} // namespace Wbsv

#endif
