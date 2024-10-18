#include "HttpRequest.hpp"

int HttpRequest::processRequestHeader(Connection& c)
{
	int rv;

	for (;;)
	{
		rv = parseRequestHeaderLine();
		if (rv == WbsvSuccess)
			continue;
		else if (rv == WbsvDone)
			return OK;
		else // WbsvAgain
		{
			int currentHeaderSize = headerIn.size() - requestLineLen;
			int leftAvailableHeaderSize = largeClientHeaderSize - currentHeaderSize;
			if (leftAvailableHeaderSize > 0)
			{
				char largeTmp[leftAvailableHeaderSize];
				std::memset(tmp, 0, leftAvailableHeaderSize);
				readnum = recv(c.cfd, largeTmp, leftAvailableHeaderSize, 0);
				if (readnum < 0)
				{
					// 500 (Internal Server Error) error
					return AGAIN;
				}
				std::string largeBuf(largeTmp, readnum);
				std::copy(largeBuf.begin(), largeBuf.end(), std::back_inserter(headerIn));
			}
			else
			{
				// 414 (Request-URI Too Large) error
				return ERROR;
			}
		}
	}
}

int HttpRequest::parseRequestLine()
{
	int state;
	switch (state)
	{
	case newLine:
		break;
	default:
		break;
	}
}

int HttpRequest::processRequestLine(Connection& c)
{
	int rv;
	for (;;)
	{
		rv = parseRequestLine();
		if (rv == WbsvSuccess)
		{
			/* processRequestUri(); */
			/* processValidataHost(); */
			/* setVirtualServer(); */
			processRequestHeader(c);
			break;
		}
		else if (rv == WbsvFailure)
			break;
		else // WbsvAgain
		{
			if (headerIn.size() < largeClientHeaderSize)
			{
				char largeTmp[largeClientHeaderSize - clientHeaderSize];
				std::memset(tmp, 0, largeClientHeaderSize - clientHeaderSize);
				readnum = recv(c.cfd, largeTmp, largeClientHeaderSize - clientHeaderSize);
				if (readnum < 0)
				{
					revHandler = processRequestLine;
					return AGAIN;
				}
				std::string largeBuf(largeTmp, readnum);
				std::copy(largeBuf.begin(), largeBuf.end(), std::back_inserter(headerIn));
			}
			else
			// 414 (Request-URI Too Large) error
		}
	}
	return OK;
}

int HttpRequest::waitRequestHandler(Connection& c)
{
	char tmp[clientHeaderSize];
	std::memset(tmp, 0, clientHeaderSize);
	ssize_t readnum = recv(c.cfd, tmp, clientHeaderSize);
	if (readnum <= 0)
	{
		// 500 (Internal Server Error) error
		return WbsvFailure;
	}
	std::string buf(tmp, readnum);
	std::copy(buf.begin(), buf.end(), std::back_inserter(headerIn));
	return processRequestLine(c);
}
