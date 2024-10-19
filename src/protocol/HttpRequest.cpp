#include "HttpRequest.hpp"

using namespace Wbsv;

int HttpRequest::parseRequestHeaderLine()
{
#ifdef DEBUG
	std::cout << "Http::parseRequestHeaderLine" << std::endl;
#endif
	return DONE;
}

int HttpRequest::processRequestHeader(Connection& c)
{
#ifdef DEBUG
	std::cout << "Http::processRequestHeader" << std::endl;
#endif
	int rv;

	for (;;)
	{
		rv = parseRequestHeaderLine();
		if (rv == OK)
			continue;
		else if (rv == DONE)
			return OK;
		else // AGAIN || ERROR
		{
			int currentHeaderSize = headerIn.size() - requestLineLen;
			int leftAvailableHeaderSize = largeClientHeaderSize - currentHeaderSize;
			if (leftAvailableHeaderSize > 0)
			{
				char largeTmp[leftAvailableHeaderSize];
				std::memset(largeTmp, 0, leftAvailableHeaderSize);
				int readnum = recv(c.cfd, largeTmp, leftAvailableHeaderSize, 0);
				if (readnum < 0)
					return AGAIN_REQUESTHEADER;
				std::string largeBuf(largeTmp, readnum);
#ifndef DEBUG
				std::cout << "after recv() in processRequestHeader():" << std::endl;
				std::cout << "size: " << std::endl << largeBuf.size() << std::endl;
				std::cout << "largeBuf: " << std::endl << largeBuf << std::endl;
#endif
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
#ifdef DEBUG
	std::cout << "Http::parseRequestLine" << std::endl;
#endif
	return OK;
}

int HttpRequest::processRequestLine(Connection& c)
{
#ifdef DEBUG
	std::cout << "Http::processRequestLine" << std::endl;
#endif
	int rv;
	for (;;)
	{
		rv = parseRequestLine();
		if (rv == OK)
		{
			/* processRequestUri(); */
			/* processValidataHost(); */
			/* setVirtualServer(); */
			return processRequestHeader(c);
		}
		else if (rv == ERROR)
			return ERROR;
		else // WbsvAgain
		{
			if (headerIn.size() < largeClientHeaderSize)
			{
				char largeTmp[largeClientHeaderSize - clientHeaderSize];
				std::memset(largeTmp, 0, largeClientHeaderSize - clientHeaderSize);
				int readnum = recv(c.cfd, largeTmp, largeClientHeaderSize - clientHeaderSize, 0);
				if (readnum < 0)
					return AGAIN_REQUESTLINE;
				std::string largeBuf(largeTmp, readnum);
#ifdef DEBUG
				std::cout << "after recv() in processRequestLine():" << std::endl;
				std::cout << "size: " << std::endl << largeBuf.size() << std::endl;
				std::cout << "largeBuf: " << std::endl << largeBuf << std::endl;
#endif
				std::copy(largeBuf.begin(), largeBuf.end(), std::back_inserter(headerIn));
			}
			else
				return ERROR;
			// 414 (Request-URI Too Large) error
		}
	}
}
