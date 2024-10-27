#include "Http.hpp"

using namespace Wbsv;

void Http::initPhaseHandler()
{
	ph.push_back(new FindConfig);
	ph.push_back(new Rewrite);
	ph.push_back(new Access);
	ph.push_back(new CgiContent);
	ph.push_back(new Content);
}

int Http::invokeRevHandler()
{
#ifdef DEBUG
	std::cout << "Invoking revHandler: " << std::endl;
#endif
	return (this->*(revHandler))();
}

void Http::selectServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls)
{
	std::vector<ConfCtx*>::iterator it;

	for (it = cfs->begin(); it != cfs->end(); it++)
	{
		ConfCtx* ctx = *it;
		if (ctx->getProtocol() == "HTTP")
		{
			HttpConfCtx* hc = reinterpret_cast<HttpConfCtx*>(ctx);
			std::vector<ServerCtx>::iterator sit;
			for (sit = hc->getServerCtxs().begin(); sit != hc->getServerCtxs().end(); sit++)
			{
				struct addrinfo hints;
				struct addrinfo* result;
				std::memset(&hints, 0, sizeof(struct addrinfo));
				/* you can use SOCK_DGRAM if add StreamConfCtx, but not HttpConfCtx */
				hints.ai_flags = AI_NUMERICSERV;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_family = AF_INET;
				hints.ai_canonname = NULL;
				hints.ai_addr = NULL;
				hints.ai_next = NULL;
				std::pair<string, string> listen = sit->getListen();
				if (getaddrinfo(listen.first.c_str(), listen.second.c_str(), &hints, &result) != 0)
				{
					std::cerr << strerror(errno) << std::endl;
					exit(1);
				}
				struct sockaddr_in* addrIn = (struct sockaddr_in*)result->ai_addr;
				if (addrIn->sin_port == ls->sockaddrIn.sin_port &&
					addrIn->sin_addr.s_addr == ls->sockaddrIn.sin_addr.s_addr)
				{
					serverCtx = &(*sit);
					c.serverCtx = serverCtx;
#ifdef DEBUG
					std::cout << "Listening socket (" << ls->sfd
							  << ") is belong to the server which is ..." << std::endl;
					std::cout << "\tlisten: " << listen.first << ":" << listen.second << std::endl;
#endif
				}
				freeaddrinfo(result);
			}
		}
	}
}

void Http::selectVServerCtx(ServerCtx* serverCtx, string host)
{
#ifdef DEBUG
	std::cout << "selectVServerCtx" << std::endl;
#endif
	std::vector<VServerCtx>* v = serverCtx->getVServerCtxs();
	std::vector<VServerCtx>::iterator it;
	std::vector<VServerCtx>::iterator defaultServer;
	for (it = v->begin(); it != v->end(); it++)
	{
		if (it->serverName == host)
		{
			vserverCtx_ = &(*it);
#ifdef DEBUG
			std::cout << "Host: " << host << " matched" << std::endl;
#endif
			return;
		}
		if (it->defaultServer)
			defaultServer = it;
	}
#ifdef DEBUG
	std::cout << "Default server used" << std::endl;
#endif
	vserverCtx_ = &(*defaultServer);
}

int Http::waitRequestHandler()
{
#ifdef DEBUG
	std::cout << "Http::waitRequestHandler" << std::endl;
#endif
	char tmp[clientHeaderSize + 1];
	std::memset(tmp, 0, clientHeaderSize + 1);
	ssize_t readnum = recv(c.cfd, tmp, clientHeaderSize, 0);
#ifdef DEBUG
	std::cout << "after recv:" << std::endl;
	std::cout << "\tmaxsize of recv: " << clientHeaderSize << std::endl;
	std::cout << "\treadnum: " << readnum << std::endl;
#endif
	if (readnum < 0)
	{
		// 500 (Internal Server Error) error
		return ERROR;
	}
	alreadyRead = true;
	if (readnum >= clientHeaderSize)
		ready = true;
	string buf(tmp, readnum);
	headerIn += buf;
	return processRequestLine();
}

int Http::processRequestLine()
{
#ifdef DEBUG
	std::cout << "Http::processRequestLine" << std::endl;
	std::cout << "Current state of Request object: " << std::endl;
	std::cout << "pos: " << pos << std::endl;
	std::cout << "state: " << state << std::endl;
	std::cout << "headerIn: " << headerIn << std::endl;
#endif
	int rv = AGAIN;
	for (;;)
	{
		if (rv == AGAIN)
		{
			if (!alreadyRead)
			{
				int bufSize = largeClientHeaderSize - clientHeaderSize;
				char buf[bufSize + 1];
				std::memset(buf, 0, bufSize + 1);
				ssize_t readnum = recv(c.cfd, buf, bufSize, 0);
#ifdef DEBUG
				std::cout << "after recv:" << std::endl;
				std::cout << "\tmaxsize of recv: " << bufSize << std::endl;
				std::cout << "\treadnum: " << readnum << std::endl;
#endif
				if (readnum < 0)
					return ERROR;
				/* if (readnum >= bufSize) */
				/* { */
				/* 	std::cout << "Request-URI Too Large" << std::endl; */
				/* 	return ERROR; */
				/* } */
				if (readnum >= bufSize)
					ready = true;
				alreadyRead = true;
				headerIn += buf;
			}
		}
		rv = parseRequestLine();
		std::cout << rv << std::endl;
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
			requestLineLen = pos;
			/* processRequestUri(); */
			/* processValidataHost(); */
			/* setVirtualServer(); */
			return processRequestHeader();
		}
		else if (rv != AGAIN)
		{
#ifdef DEBUG
			std::cout << "Error return after parseRequestLine" << std::endl;
#endif
			return ERROR;
		}
		else // AGAIN
		{
			if (headerIn.size() >= largeClientHeaderSize)
			{
				std::cout << "Request-URI Too Large" << std::endl;
				return ERROR;
			}
			if (!ready)
			{
				/* Bad Request (Parse Error) */
				std::cout << "Bad Request" << std::endl;
				return ERROR;
			}
			ready = false;
			alreadyRead = false;
			revHandler = &Http::processRequestLine;
			return AGAIN;
		}
	}
}

int Http::processRequestHeader()
{
#ifdef DEBUG
	std::cout << "Http::processRequestHeader" << std::endl;
#endif
	int rv = AGAIN;

	for (;;)
	{
		if (rv == AGAIN)
		{
			if (!alreadyRead)
			{
				int currentHeaderSize = headerIn.size() - requestLineLen;
				int leftAvailableHeaderSize = largeClientHeaderSize - currentHeaderSize;
				char buf[leftAvailableHeaderSize + 1];
				std::memset(buf, 0, leftAvailableHeaderSize + 1);
				ssize_t readnum = recv(c.cfd, buf, leftAvailableHeaderSize, 0);
#ifdef DEBUG
				std::cout << "after recv:" << std::endl;
				std::cout << "\tmaxsize of recv: " << leftAvailableHeaderSize << std::endl;
				std::cout << "\treadnum: " << readnum << std::endl;
#endif
				if (readnum < 0)
					return ERROR;
				/* if (readnum >= leftAvailableHeaderSize) */
				/* { */
				/* 	std::cout << "Request-Header Too Large (Bad Request)" << std::endl; */
				/* 	return ERROR; */
				/* } */
				if (readnum >= leftAvailableHeaderSize)
					ready = true;
				alreadyRead = true;
				headerIn += buf;
			}
		}
		rv = parseRequestHeaderLine();
		if (rv == OK)
		{
			map<string, string>::iterator it;
			it = headersIn.find(headerFieldNameTmp);
			if (it == headersIn.end())
				headersIn[headerFieldNameTmp] = headerFieldValueTmp;
			else
			{
				if (it->second[it->second.size() - 1] != ',')
					return ERROR;
				it->second += headerFieldValueTmp;
			}
			if (headerFieldNameTmp == "Host")
				selectVServerCtx(c.serverCtx, headerFieldValueTmp);
			headerFieldNameTmp = "";
			headerFieldValueTmp = "";
			continue;
		}
		else if (rv == DONE)
		{
#ifdef DEBUG
			map<string, string>::iterator it;
			std::cout << "Request-header is parsed" << std::endl;
			for (it = headersIn.begin(); it != headersIn.end(); it++)
			{
				std::cout << "{ " << it->first << " : " << it->second << " }" << std::endl;
			}
#endif
			return processRequest();
		}
		else if (rv != AGAIN)
			return ERROR;
		else // AGAIN
		{
			if (headerIn.size() - requestLineLen >= largeClientHeaderSize)
			{
				std::cout << "Request-Header Too Large (Bad Request)" << std::endl;
				return ERROR;
			}
			if (!ready)
			{
				/* Bad Request (Parse Error) */
				std::cout << "Bad Request" << std::endl;
				return ERROR;
			}
			ready = false;
			alreadyRead = false;
			revHandler = &Http::processRequestHeader;
			return AGAIN;
		}
	}
}

int Http::parseRequestLine()
{
#ifdef DEBUG
	std::cout << "Http::parseRequestLine" << std::endl;
#endif
	enum lineState
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

	for (; pos < headerIn.size(); pos++)
	{
		switch (state)
		{
		case METHOD:
			if (std::isupper(headerIn[pos]))
				break;
			else if (headerIn[pos] == SP)
			{
				string tmp(headerIn, 0, pos);
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
				string tmp(headerIn, start, pos - start);
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
			state = 0;
			return OK;
		}
	}
	if (state == END)
	{
		state = 0;
		return OK;
	}
	return AGAIN;
}

int Http::parseRequestHeaderLine()
{
#ifdef DEBUG
	std::cout << "Http::parseRequestHeaderLine" << std::endl;
#endif
	enum headerState
	{
		START = 0,
		FIELD_NAME,
		COLON,
		FIRST_FIELD_VALUE,
		FIELD_VALUE,
		LINE_ALMOST_DONE,
		LINE_DONE,
		HEADER_ALMOST_DONE,
		HEADER_DONE
	};

	for (; pos < headerIn.size(); pos++)
	{
		switch (state)
		{
		case START:
			start = pos;
			if (headerIn[pos] == CR)
			{
				state = HEADER_ALMOST_DONE;
				break;
			}
			if (std::iscntrl(headerIn[pos]))
				return INVALID_HEADER;
			switch (headerIn[pos])
			{
			case '(':
			case ')':
			case '<':
			case '>':
			case '@':
			case ',':
			case ';':
			case ':':
			case '\\':
			case '\"':
			case '/':
			case '[':
			case ']':
			case '?':
			case '=':
			case '{':
			case '}':
			case SP:
				return INVALID_HEADER;
			}
			state = FIELD_NAME;
			break;
		case FIELD_NAME:
			if (std::iscntrl(headerIn[pos]))
				return INVALID_HEADER;
			switch (headerIn[pos])
			{
			case '(':
			case ')':
			case '<':
			case '>':
			case '@':
			case ',':
			case ';':
			case '\\':
			case '\"':
			case '/':
			case '[':
			case ']':
			case '?':
			case '=':
			case '{':
			case '}':
			case SP:
				return INVALID_HEADER;
			}
			if (headerIn[pos] == ':')
			{
				string tmp(headerIn, start, pos - start);
				headerFieldNameTmp = tmp;
				state = FIRST_FIELD_VALUE;
			}
			break;
		case FIRST_FIELD_VALUE:
			/* it may be not sufficient */
			start = pos;
			if (headerIn[pos] == CR)
			{
				headerFieldValueTmp = "";
				state = LINE_ALMOST_DONE;
				break;
			}
			if (headerIn[pos] == SP)
				break;
			state = FIELD_VALUE;
			break;
		case FIELD_VALUE:
			/* it may be not sufficient */
			if (headerIn[pos] == CR)
			{
				string tmp(headerIn, start, pos - start);
				headerFieldValueTmp = tmp;
				state = LINE_ALMOST_DONE;
				break;
			}
			break;
		case LINE_ALMOST_DONE:
			if (headerIn[pos] != LF)
				return INVALID_HEADER;
			state = LINE_DONE;
			break;
		case LINE_DONE:
			state = 0;
			return OK;
		case HEADER_ALMOST_DONE:
			if (headerIn[pos] != LF)
				return INVALID_HEADER;
			state = HEADER_DONE;
			break;
		case HEADER_DONE:
			return DONE;
		}
	}
	if (state == HEADER_DONE)
		return DONE;
	return AGAIN;
}

int Http::processRequest()
{
	std::cout << "processRequest" << std::endl;
	/* int fd = open("/root/webserv/test/html/index.html", O_RDONLY); */
	/* if (fd < 0) */
	/* 	return; */
	/* string responseHeader("HTTP/1.1 200 OK\r\n\r\n"); */
	/* write(c.cfd, responseHeader.c_str(), responseHeader.size()); */
	/* int bufSize = 1024; */
	/* char buf[bufSize]; */
	/* for (;;) */
	/* { */
	/* 	std::memset(buf, 0, bufSize); */
	/* 	ssize_t readnum = read(fd, buf, bufSize); */
	/* 	if (readnum <= 0) */
	/* 		break; */
	/* 	write(c.cfd, buf, readnum); */
	/* } */
	return coreRunPhase();
	/* finalizeRequest(); */
	/* finalizeConnection(); */
}

int Http::coreRunPhase()
{
#ifdef DEBUG
	std::cout << "coreRunPhase" << std::endl;
#endif
	std::vector<PhaseHandler*>::iterator it;
	for (it = ph.begin(); it != ph.end(); it++)
	{
		if ((*it)->checker(*this) == DONE)
			break;
	}
	return finalizeRequest();
}

int Http::finalizeRequest()
{
#ifdef DEBUG
	std::cout << "finalizeRequest" << std::endl;
#endif
	return OK;
}
