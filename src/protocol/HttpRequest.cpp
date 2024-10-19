#include "HttpRequest.hpp"

using namespace Wbsv;

int HttpRequest::parseRequestHeaderLine()
{
#ifdef DEBUG
	std::cout << "Http::parseRequestHeaderLine" << std::endl;
#endif
	for (; pos < headerIn.size(); pos++)
	{
	}
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
				headerIn += largeBuf;
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
	int start = 0;
	for (; pos < headerIn.size(); pos++)
	{
		switch (state)
		{
		case METHOD:
			if (std::isupper(headerIn[pos]))
				break;
			else if (headerIn[pos] == SP)
			{
				std::string tmp(headerIn, 0, pos);
				if (tmp == "GET")
					method = GET;
				else if (tmp == "POST")
					method = POST;
				else if (tmp == "DELETE")
					method = DELETE;
				else
					return INVALID_METHOD;
				state = SLASH_IN_URI;
				break;
			}
			else
				return INVALID_METHOD;
		case SLASH_IN_URI:
			start = pos;
			if (headerIn[start] != '/')
				return INVALID_URI;
			state = SEGMENT;
			break;
		case SEGMENT:
			if (std::isalnum(headerIn[pos]))
				break;
			/* else if (headerIn[pos] == '%') */
			/* { */
			/* 	state = HEX; */
			/* 	break; */
			/* } */
			else if (headerIn[pos] == SP)
			{
				state = VERSION_H;
				std::string tmp(headerIn, start, pos - start);
				uri = tmp;
				start = pos + 1;
				break;
			}
			else if (headerIn[pos] == '/')
			{
				if (headerIn[pos - 1] == '/')
					return INVALID_URI;
				break;
			}
			switch (headerIn[pos])
			{
			case ':':
			case '@':
			case '&':
			case '=':
			case '+':
			case '$':
			case ',':
			case '-':
			case '_':
			case '.':
			case '!':
			case '~':
			case '*':
			case '\'':
			case '(':
			case ')':
			case ';': // it may be out of rules
				break;
			default:
				return INVALID_URI;
			}
		case VERSION_H:
			if (headerIn[pos] != 'H')
				return INVALID_VERSION;
			state = VERSION_HT;
			break;
		case VERSION_HT:
			if (headerIn[pos] != 'T')
				return INVALID_VERSION;
			state = VERSION_HTT;
			break;
		case VERSION_HTT:
			if (headerIn[pos] != 'T')
				return INVALID_VERSION;
			state = VERSION_HTTP;
			break;
		case VERSION_HTTP:
			if (headerIn[pos] != 'P')
				return INVALID_VERSION;
			state = VERSION_SLASH;
			break;
		case VERSION_SLASH:
			if (headerIn[pos] != '/')
				return INVALID_VERSION;
			state = VERSION_MAJOR;
			break;
		case VERSION_MAJOR:
			if (!std::isdigit(headerIn[pos]))
				return INVALID_VERSION;
			// 505 VERSION_NOT_SUPPORTED
			major = headerIn[pos] - '0';
			state = VERSION_POINT;
			break;
		case VERSION_POINT:
			if (headerIn[pos] != '.')
				return INVALID_VERSION;
			state = VERSION_MINOR;
			break;
		case VERSION_MINOR:
			if (!std::isdigit(headerIn[pos]))
				return INVALID_VERSION;
			// 505 VERSION_NOT_SUPPORTED
			minor = headerIn[pos] - '0';
			state = END_CR;
			break;
		case END_CR:
			if (headerIn[pos] != CR)
				return INVALID_VERSION;
			state = END_LF;
			break;
		case END_LF:
			if (headerIn[pos] != LF)
				return INVALID_VERSION;
			state = END;
			break;
		case END:
			return OK;
		}
	}
	return AGAIN;
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
#ifdef DEBUG
			std::cout << "Request-line is parsed" << std::endl;
			std::cout << "Method: "
					  << (method == GET	   ? "GET"
						  : method == POST ? "POST"
										   : "DELETE")
					  << std::endl;
			std::cout << "URI: " << uri << std::endl;
			std::cout << "Version: " << major << "." << minor << std::endl;
#endif
			/* processRequestUri(); */
			/* processValidataHost(); */
			/* setVirtualServer(); */
			return processRequestHeader(c);
		}
		else if (rv != AGAIN)
			return ERROR;
		else // AGAIN
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
				headerIn += largeBuf;
			}
			else
				return ERROR;
			// 414 (Request-URI Too Large) error
		}
	}
}
