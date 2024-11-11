#include "Http.hpp"

using namespace Wbsv;

Http::Http()
	: clientHeaderSize(1024)
	, largeClientHeaderSize(8192)
	, requestLineLen(0)
	, pos(0)
	, start(0)
	, state(0)
	, method(0)
	, major(0)
	, minor()
	, alreadyRead(false)
	, alreadyWrite(false)
	, ready(false)
	, responseState(0)
	, continueRequest_(false)
	, fd_(-1)
	, requestBodyFileFd_(-1)
	, responseBodyFileFd_(-1)
	, completelyRead(true)
	, otherThanChunkDataSize_(0)
	, unchunkedRequestSize_(0)
	, chunkSize_(0)
	, countChunkData_(0)
{
	std::memset(responseBodyBuf_, 0, 1024);
	std::memset(chunkedRequestBuf_, 0, 1025);
	std::memset(unchunkedRequestBuf_, 0, 1025);
	defaultErrorPages["400"] = "<html>\r\n<head><title>400 Bad "
							   "Request</title></head>\r\n<body>\r\n<center><h1>400 Bad "
							   "Request</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["404"] = "<html><html>\r\n<head><title>404 Not "
							   "Found</title></head>\r\n<body>\r\n<center><h1>404 Not "
							   "Found</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["405"] = "<html>\r\n<head><title>405 Not "
							   "Allowed</title></head>\r\n<body>\r\n<center><h1>405 Not "
							   "Allowed</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["406"] = "<html>\r\n<head><title>406 Not "
							   "Acceptable</title></head>\r\n<body>\r\n<center><h1>406 Not "
							   "Acceptable</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["408"] = "<html>\r\n<head><title>408 Request "
							   "Timeout</title></head>\r\n<body>\r\n<center><h1>408 Request "
							   "Timeout</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["413"] = "<html>\r\n<head><title>413 Request "
							   "Entity Too Large</title></head>\r\n<body>\r\n<center><h1>413 "
							   "Request Entity Too Large</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["414"] = "<html>\r\n<head><title>414 Request-"
							   "Line Too Long</title></head>\r\n<body>\r\n<center><h1>414 "
							   "Request-Line Too Long</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["415"] = "<html>\r\n<head><title>415 Unsupported "
							   "Media Type</title></head>\r\n<body>\r\n<center><h1>415 Unsupported "
							   "Media Type</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["417"] = "<html>\r\n<head><title>417 Expectation "
							   "Failed</title></head>\r\n<body>\r\n<center><h1>417 Expectation "
							   "Failed</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["500"] = "<html>\r\n<head><title>500 Internal "
							   "Server Error</title></head>\r\n<body>\r\n<center><h1>500 Internal "
							   "Server Error</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["501"] =
		"<html>\r\n<head><title>501 Not Implemented</title></head>\r\n<body>\r\n<center><h1>501 "
		"Not Implemented</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["502"] = "<html>\r\n<head><title>502 Bad "
							   "Gateway</title></head>\r\n<body>\r\n<center><h1>502 Bad "
							   "Gateway</h1></center>\r\n</body>\r\n</html>\r\n";
	defaultErrorPages["505"] = "<html>\r\n<head><title>505 HTTP "
							   "Version Not Supported</title></head>\r\n<body>\r\n<center><h1>505 "
							   "HTTP Version Not Supported</h1></center>\r\n</body>\r\n</html>\r\n";
}

void Http::initPhaseHandler()
{
	ph.push_back(new FindConfig);
	ph.push_back(new Rewrite);
	ph.push_back(new Access);
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
					std::cerr << "getaddrinfo: " << strerror(errno) << std::endl;
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
	std::vector<VServerCtx>* vserverCtxs = serverCtx->getVServerCtxs();
	std::vector<VServerCtx>::iterator vsit = vserverCtxs->begin();
	for (; vsit != vserverCtxs->end(); vsit++)
	{
		if (vsit->defaultServer)
		{
			vserverCtx_ = &(*vsit);
			break;
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
	// wevReady = true;
	char tmp[clientHeaderSize + 1];
	std::memset(tmp, 0, clientHeaderSize + 1);
	ssize_t readnum = recv(c.cfd, tmp, clientHeaderSize, 0);
	c.lastReadTime = std::time(NULL);
#ifdef DEBUG
	std::cout << "after recv:" << std::endl;
	std::cout << "\tmaxsize of recv: " << clientHeaderSize << std::endl;
	std::cout << "\treadnum: " << readnum << std::endl;
#endif
	if (readnum < 0)
	{
		// 500 (Internal Server Error) error
		return finalizeRequest();
	}
	else if (readnum == 0)
		return ERROR;
	alreadyRead = true;
	if (readnum == clientHeaderSize)
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
				c.lastReadTime = std::time(NULL);
#ifdef DEBUG
				std::cout << "after recv:" << std::endl;
				std::cout << "\tmaxsize of recv: " << bufSize << std::endl;
				std::cout << "\treadnum: " << readnum << std::endl;
#endif
				if (readnum < 0)
				{
					// 500 (Internal Server Error) error
					return finalizeRequest();
				}
				else if (readnum == 0)
					return ERROR;
				if (readnum == bufSize)
					ready = true;
				alreadyRead = true;
				headerIn += buf;
			}
		}
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
			statusLine = "HTTP/1.1 400 Bad Request\r\n";
			headerOut = "\r\n";
			messageBodyOut = defaultErrorPages["400"];
			completelyRead = false;
			return finalizeRequest();
		}
		else // AGAIN
		{
			if (headerIn.size() >= largeClientHeaderSize)
			{
				statusLine = "HTTP/1.1 414 Request-Line Too Long\r\n";
				headerOut = "\r\n";
				messageBodyOut = defaultErrorPages["414"];
				return finalizeRequest();
			}
			if (!ready)
			{
				statusLine = "HTTP/1.1 400 Bad Request\r\n";
				headerOut = "\r\n";
				messageBodyOut = defaultErrorPages["400"];
				return finalizeRequest();
			}
			ready = false;
			alreadyRead = false;
			revHandler = &Http::processRequestLine;
			return AGAIN;
		}
	}
}

bool isLargerThanMaxBodySize(size_t max, std::string& bodySize)
{
	int body = std::atoi(bodySize.c_str());
	if ((size_t)body > max)
		return true;
	return false;
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
				c.lastReadTime = std::time(NULL);
#ifdef DEBUG
				std::cout << "after recv:" << std::endl;
				std::cout << "\tmaxsize of recv: " << leftAvailableHeaderSize << std::endl;
				std::cout << "\treadnum: " << readnum << std::endl;
#endif
				if (readnum < 0)
				{
					// 500 (Internal Server Error) error
					return finalizeRequest();
				}
				else if (readnum == 0)
					return ERROR;
				/* if (readnum >= leftAvailableHeaderSize) */
				/* { */
				/* 	std::cout << "Request-Header Too Large (Bad Request)" << std::endl; */
				/* 	return ERROR; */
				/* } */
				if (readnum == leftAvailableHeaderSize)
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
			if (headerFieldNameTmp == "Transfer-Encoding" && headerFieldValueTmp != "chunked")
			{
				// 501 (Unimplemented)
				statusLine = "HTTP/1.1 501 Not Implemented\r\n";
				headerOut = "\r\n";
				messageBodyOut = defaultErrorPages["501"];
				return finalizeRequest();
			}
			headerFieldNameTmp = "";
			headerFieldValueTmp = "";
			continue;
		}
		else if (rv == DONE)
		{
			std::map<std::string, std::string>::iterator result;
			if ((result = headersIn.find("Content-Length")) != headersIn.end())
			{
				if (isLargerThanMaxBodySize(vserverCtx_->clientMaxBodySize, result->second))
				{
					statusLine = "HTTP/1.1 413 Request Entity Too Large\r\n";
					std::stringstream size;
					size << result->second.size();
					headerOut += "Content-Length: " + size.str() + "\r\n";
					headerOut += "Content-type: text/html\r\n\r\n";
					messageBodyOut = defaultErrorPages["413"];
					return finalizeRequest();
				}
			}
			if ((result = headersIn.find("Expect")) != headersIn.end())
			{
				if (result->second == "100-continue")
				{
					statusLine = "HTTP/1.1 100 Continue\r\n";
					headerOut = "\r\n";
					continueRequest_ = true;
					return finalizeRequest();
				}
				else
				{
					statusLine = "HTTP/1.1 417 Expectation Failed\r\n";
					headerOut = "\r\n";
					messageBodyOut = defaultErrorPages["417"];
					return finalizeRequest();
				}
			}
#ifdef DEBUG
			map<string, string>::iterator it;
			std::cout << "Request-header is parsed" << std::endl;
			for (it = headersIn.begin(); it != headersIn.end(); it++)
			{
				std::cout << "{ " << it->first << " : " << it->second << " }" << std::endl;
			}
#endif
			// pos = 0;
			return processRequest();
		}
		else if (rv != AGAIN)
		{
			statusLine = "HTTP/1.1 400 Bad Requet\r\n";
			headerOut = "\r\n";
			messageBodyOut = defaultErrorPages["400"];
			return finalizeRequest();
		}
		else // AGAIN
		{
			if (headerIn.size() - requestLineLen >= largeClientHeaderSize)
			{
				statusLine = "HTTP/1.1 400 Bad Requet\r\n";
				headerOut = "\r\n";
				messageBodyOut = defaultErrorPages["400"];
				return finalizeRequest();
			}
			if (!ready)
			{
				statusLine = "HTTP/1.1 400 Bad Requet\r\n";
				headerOut = "\r\n";
				messageBodyOut = defaultErrorPages["400"];
				return finalizeRequest();
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
			break;
		case VERSION_H:
			if (headerIn[pos] != 'H')
			{
				std::cout << "VERSION_H: " << headerIn[pos] << std::endl;
				return INVALID_VERSION;
			}
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
			state = 0;
			return DONE;
		}
	}
	if (state == HEADER_DONE)
	{
		state = 0;
		return DONE;
	}
	return AGAIN;
}

int Http::parseChunkedRequest(const char* buf, size_t size)
{
#ifdef DEBUG
	std::cout << "parseChunkedRequest" << std::endl;
#endif
	enum chunkedRequestState
	{
		START = 0,
		CHUNK_SIZE,
		CHUNK_EXTENSION,
		CHUNK_SIZE_ALMOST_DONE,
		CHUNK_DATA,
		CHUNK_DATA_ALMOST_DONE,
		LAST_CHUNK_EXTENSION,
		LAST_CHUNK_ALMOST_DONE
	};
	otherThanChunkDataSize_ = 0;
	unchunkedRequestSize_ = 0;
	std::cout << "state: " << state << std::endl;
	for (int i = 0; i < size; i++)
	{
		switch (state)
		{
		case START:
#ifdef DEBUG
			std::cout << "START" << std::endl;
#endif
			if (!std::isdigit(buf[i]) && !std::isxdigit(buf[i]))
				return ERROR;
			otherThanChunkDataSize_++;
			chunkSizeStr_ = buf[i];
			state = CHUNK_SIZE;
			break;
		case CHUNK_SIZE:
#ifdef DEBUG
			std::cout << "CHUNK_SIZE" << std::endl;
#endif
			otherThanChunkDataSize_++;
			if (buf[i] == CR)
			{
				chunkSizeStr_ += buf[i];
				std::stringstream ss;
				ss << std::hex << chunkSizeStr_;
				// ss << tmp;
				ss >> chunkSize_;
				// chunkSize_ += 100;
#ifdef DEBUG
				std::cout << "chunk-size: " << chunkSize_ << std::endl;
#endif
				if (chunkSize_ == 0)
					state = LAST_CHUNK_ALMOST_DONE;
				else
					state = CHUNK_SIZE_ALMOST_DONE;
				break;
			}
			else if (buf[i] == ';')
			{
				otherThanChunkDataSize_++;
				std::string tmp(buf, i - start);
				std::stringstream ss;
				ss << std::hex << tmp;
				ss >> chunkSize_;
				if (chunkSize_ == 0)
					state = LAST_CHUNK_EXTENSION;
				else
					state = CHUNK_EXTENSION;
				break;
			}
			if (!std::isxdigit(buf[i]))
				return ERROR;
			chunkSizeStr_ += buf[i];
			break;
		case CHUNK_EXTENSION: // does not support "chunk-extension".
#ifdef DEBUG
			std::cout << "CHUNK_EXTENSION" << std::endl;
#endif
			otherThanChunkDataSize_++;
			if (buf[i] == CR)
			{
				state = CHUNK_SIZE_ALMOST_DONE;
				break;
			}
			break;
		case CHUNK_SIZE_ALMOST_DONE:
#ifdef DEBUG
			std::cout << "CHUNK_SIZE_ALMOST_DONE" << std::endl;
#endif
			if (buf[i] != LF)
				return ERROR;
			otherThanChunkDataSize_++;
			countChunkData_ = 0;
			state = CHUNK_DATA;
			break;
		case CHUNK_DATA:
			// #ifdef DEBUG
			// 			std::cout << countChunkData_ << std::endl;
			// #endif
			if (countChunkData_ == chunkSize_)
			{
				if (buf[i] != CR)
					return ERROR;
				otherThanChunkDataSize_++;
				state = CHUNK_DATA_ALMOST_DONE;
				break;
			}
			unchunkedRequestBuf_[unchunkedRequestSize_++] = buf[i];
			countChunkData_++;
			break;
		case CHUNK_DATA_ALMOST_DONE:
#ifdef DEBUG
			std::cout << "chunk-data almost done" << std::endl;
#endif
			if (buf[i] != LF)
				return ERROR;
			otherThanChunkDataSize_++;
			state = START;
			break;
		case LAST_CHUNK_EXTENSION: // does not support "chunk-extension".
#ifdef DEBUG
			std::cout << "LAST_CHUNK_EXTENSION" << std::endl;
#endif
			otherThanChunkDataSize_++;
			if (buf[i] == CR)
			{
				state = LAST_CHUNK_ALMOST_DONE;
				break;
			}
			break;
		case LAST_CHUNK_ALMOST_DONE:
			if (buf[i] != LF)
				return ERROR;
			otherThanChunkDataSize_++;
#ifdef DEBUG
			std::cout << "parsing chunked request done" << std::endl;
#endif
			return DONE;
			// does not support "trailar".
		}
	}
	return AGAIN;
}

int Http::processRequest()
{
	revHandler = &Http::processRequest;
#ifdef DEBUG
	std::cout << "processRequest" << std::endl;
#endif
	if (headersIn.find("Transfer-Encoding") != headersIn.end()) // chunked Transfer-Encoding
	{
#ifdef DEBUG
		std::cout << "process transfer-encoding" << std::endl;
#endif
		int rv = DONE;
		if (alreadyRead)
		{
			if (headerIn.size() - pos > vserverCtx_->clientMaxBodySize)
			{
				statusLine = "HTTP/1.1 413 Request Entity Too Large\r\n";
				headerOut = "\r\n";
				messageBodyOut = defaultErrorPages["413"];
				return finalizeRequest();
			}
			std::string tmpDir("/tmp/");
			std::string tmpExt(".reqBody");
			std::time_t now = std::time(NULL);
			requestBodyFileName_ = tmpDir + std::asctime(std::localtime(&now)) + tmpExt;
			requestBodyFileFd_ = open(requestBodyFileName_.c_str(), O_WRONLY | O_CREAT);
			if (requestBodyFileFd_ == -1)
			{
				// Internal Server Error
				return finalizeRequest();
			}
			int leftHeaderInSize = headerIn.size() - pos;
			int writenum = 0;
			for (; leftHeaderInSize > 0; leftHeaderInSize -= (writenum + otherThanChunkDataSize_))
			{
#ifdef DEBUG
				std::cout << "-----------------------------------------" << std::endl;
				std::cout << "leftHeaderInSize: " << leftHeaderInSize << std::endl;
				std::cout << "writenum: " << writenum << std::endl;
				std::cout << "otherThanChunkDataSize_: " << otherThanChunkDataSize_ << std::endl;
				std::cout << "leftHeaderInSize: " << leftHeaderInSize << std::endl;
				std::cout << "buffer that is going to be parsed:" << std::endl;
				std::cout << "size: "
						  << ((headerIn.size() - pos > 1024) ? 1024 : (int)(headerIn.size() - pos))
						  << std::endl;
				std::cout << headerIn.c_str() + pos << std::endl;
				std::cout << "-----------------------------------------" << std::endl;
#endif
				rv = parseChunkedRequest(headerIn.c_str() + pos,
										 (headerIn.size() - pos > 1024) ? 1024
																		: (headerIn.size() - pos));
#ifdef DEBUG
				std::cout << (rv == DONE	? "rv == DONE"
							  : rv == ERROR ? "rv == ERROR"
							  : rv == OK	? "rv == OK"
											: "rv == AGAIN")
						  << std::endl;
#endif
				if (rv == DONE)
					break;
				else if (rv == ERROR) // ERROR
				{
					statusLine = "HTTP/1.1 400 Bad Request\r\n";
					headerOut = "\r\n";
					messageBodyOut = defaultErrorPages["400"];
					close(requestBodyFileFd_);
					std::remove(requestBodyFileName_.c_str());
					return finalizeRequest();
				}
				writenum = write(requestBodyFileFd_, unchunkedRequestBuf_, unchunkedRequestSize_);
#ifdef DEBUG
				std::cout << "writenum: " << writenum << std::endl;
				std::cout << "unchunkedRequestSize_: " << unchunkedRequestSize_ << std::endl;
				std::cout << "unchunkedRequestBuf_: " << unchunkedRequestBuf_ << std::endl;
#endif
				if (writenum < 0)
				{
					// Internal Server Error
					close(requestBodyFileFd_);
					std::remove(requestBodyFileName_.c_str());
					return finalizeRequest();
				}
				std::memset(unchunkedRequestBuf_, 0, 1024);
			}
			if (rv == OK || rv == AGAIN)
			{
				alreadyRead = false;
				return AGAIN;
			}
		}
		else
		{
			struct stat st;
			if (stat(requestBodyFileName_.c_str(), &st) == -1)
			{
				// Server Internal Error
				close(requestBodyFileFd_);
				std::remove(requestBodyFileName_.c_str());
				return finalizeRequest();
			}
			ssize_t readnum = recv(c.cfd, chunkedRequestBuf_, clientHeaderSize, 0); // 1024
			if (readnum == -1)
			{
				// Server Internal Error
				close(requestBodyFileFd_);
				std::remove(requestBodyFileName_.c_str());
				return finalizeRequest();
			}
			if (readnum == 0)
				return ERROR;
			if (readnum == clientHeaderSize)
				ready = true;
			if (st.st_size + readnum > vserverCtx_->clientMaxBodySize)
			{
				statusLine = "HTTP/1.1 413 Request Entity Too Large\r\n";
				headerOut = "\r\n";
				messageBodyOut = defaultErrorPages["413"];
				close(requestBodyFileFd_);
				std::remove(requestBodyFileName_.c_str());
				return finalizeRequest();
			}
#ifdef DEBUG
			std::cout << "-----------------------------------------" << std::endl;
			std::cout << "buffer that is going to be parsed:" << std::endl;
			std::cout << "size: " << readnum << std::endl;
			std::cout << chunkedRequestBuf_ << std::endl;
			std::cout << "-----------------------------------------" << std::endl;
#endif
			rv = parseChunkedRequest(chunkedRequestBuf_, readnum);
#ifdef DEBUG
			std::cout << (rv == DONE	? "rv == DONE"
						  : rv == ERROR ? "rv == ERROR"
						  : rv == OK	? "rv == OK"
										: "rv == AGAIN")
					  << std::endl;
#endif
			if (rv == ERROR)
			{
				statusLine = "HTTP/1.1 400 Bad Request\r\n";
				headerOut = "\r\n";
				messageBodyOut = defaultErrorPages["400"];
				close(requestBodyFileFd_);
				std::remove(requestBodyFileName_.c_str());
				return finalizeRequest();
			}
			ssize_t writenum =
				write(requestBodyFileFd_, unchunkedRequestBuf_, unchunkedRequestSize_);
#ifdef DEBUG
			std::cout << "writenum: " << writenum << std::endl;
			std::cout << "unchunkedRequestSize_: " << unchunkedRequestSize_ << std::endl;
			std::cout << "unchunkedRequestBuf_: " << unchunkedRequestBuf_ << std::endl;
#endif
			if (writenum < 0)
			{
				// Internal Server Error
				close(requestBodyFileFd_);
				std::remove(requestBodyFileName_.c_str());
				return finalizeRequest();
			}
			if (rv == OK || rv == AGAIN)
			{
				std::memset(unchunkedRequestBuf_, 0, 1025);
				std::memset(chunkedRequestBuf_, 0, 1025);
				ready = false;
				return AGAIN;
			}
		}
		close(requestBodyFileFd_);
		struct stat st;
		if (stat(requestBodyFileName_.c_str(), &st) == -1)
		{
			// Server Internal Error
			std::remove(requestBodyFileName_.c_str());
			return finalizeRequest();
		}
		headersIn.erase("Transfer-Encoding");
		std::stringstream ss;
		ss << st.st_size;
		headersIn["Content-Length"] = ss.str();
	}
	else if (headersIn.find("Content-Length") != headersIn.end())
	{
#ifdef DEBUG
		std::cout << "process content-encoding" << std::endl;
#endif
		if (alreadyRead)
		{
			bodyLen_ = atoi(headersIn["Content-Length"].c_str());
			std::string tmpDir("/tmp/");
			std::string tmpExt(".reqBody");
			std::time_t now = std::time(NULL);
			requestBodyFileName_ = tmpDir + std::asctime(std::localtime(&now)) + tmpExt;
			requestBodyFileFd_ = open(requestBodyFileName_.c_str(), O_WRONLY | O_CREAT);
			if (requestBodyFileFd_ == -1)
			{
				// Server Internal Error
				return finalizeRequest();
			}
			ssize_t writenum =
				write(requestBodyFileFd_, headerIn.c_str() + pos, headerIn.size() - pos);
#ifdef DEBUG
			std::cout << "writenum: " << writenum << std::endl;
			std::cout << "body: " << std::endl << headerIn.c_str() + pos << std::endl;
#endif
			if (writenum == -1)
			{
				// Server Internal Error
				close(requestBodyFileFd_);
				std::remove(requestBodyFileName_.c_str());
				return finalizeRequest();
			}
			if (writenum < bodyLen_)
			{
				alreadyRead = false;
				return AGAIN;
			}
		}
		else
		{
			struct stat st;
			if (stat(requestBodyFileName_.c_str(), &st) == -1)
			{
				// Server Internal Error
				close(requestBodyFileFd_);
				std::remove(requestBodyFileName_.c_str());
				return finalizeRequest();
			}
			// maybe too much ?
			int leftRequestBodyLen = bodyLen_ - static_cast<int>(st.st_size);
			char buf[leftRequestBodyLen + 2];
			std::memset(buf, 0, leftRequestBodyLen + 2);
			ssize_t readnum = recv(c.cfd, buf, leftRequestBodyLen + 1, 0);
			c.lastReadTime = std::time(NULL);
			if (readnum == -1)
			{
				// Server Internal Error
				close(requestBodyFileFd_);
				std::remove(requestBodyFileName_.c_str());
				return finalizeRequest();
			}
			if (readnum == 0)
				return ERROR;
			ssize_t writenum = 0;
			if (readnum == leftRequestBodyLen + 1)
				writenum = write(requestBodyFileFd_, buf, readnum - 1);
			else
				writenum = write(requestBodyFileFd_, buf, readnum);
			if (writenum == -1)
			{
				// Internal Server Error
				close(requestBodyFileFd_);
				std::remove(requestBodyFileName_.c_str());
				return finalizeRequest();
			}
			if (readnum < leftRequestBodyLen)
			{
				ready = false;
				return AGAIN;
			}
			// when "Content-Length: 1000" but real length is 999, ready must be true because 1 byte can be read.
			if (readnum == leftRequestBodyLen + 1)
				ready = true;
		}
		close(requestBodyFileFd_);
	}
	alreadyRead = true;
	return coreRunPhase();
}

int Http::coreRunPhase()
{
#ifdef DEBUG
	std::cout << "coreRunPhase" << std::endl;
#endif
	std::vector<PhaseHandler*>::iterator it;
	int rev;
	for (it = ph.begin(); it != ph.end(); it++)
	{
		rev = (*it)->handler(*this);
		if (rev == DONE || rev == AGAIN)
			break;
	}
	if (rev == AGAIN)
		coreRunPhase();
	return processUpstream();
}

int Http::emptyHandler()
{
#ifdef DEBUG
	std::cout << "emptyHandler" << std::endl;
#endif
	return AGAIN;
}

int Http::processUpstream()
{
	if (upstream)
	{
		revHandler = &Http::emptyHandler;
#ifdef DEBUG
		std::cout << "processUpstream" << std::endl;
#endif
		return UPSTREAM_AGAIN;
	}
	return finalizeRequest();
}

int Http::finalizeRequest()
{
#ifdef DEBUG
	std::cout << "finalizeRequest" << std::endl;
#endif
	wevReady = true;
	revHandler = &Http::finalizeRequest;
	// it may be not needed
	if (!alreadyWrite)
	{
		alreadyWrite = true;
		return AGAIN;
	}

	enum state
	{
		start = 0,
		statusLineDoing,
		statusLineDone,
		headerOutDoing,
		headerOutDone,
		messageBodyDoing,
		responseDone
	};

	ssize_t writenum = 0;
	switch (responseState)
	{
	case start:
		writenum = write(c.cfd, statusLine.c_str(), statusLine.size());
		c.lastReadTime = std::time(NULL);
#ifdef DEBUG
		std::cout << "-----------------------------------------" << std::endl;
		std::cout << "sending to client (state: start):" << std::endl;
		std::cout << statusLine << std::endl;
		std::cout << "-----------------------------------------" << std::endl;
#endif
		if (writenum == -1)
			return ERROR;
		if (writenum < statusLine.size())
		{
			responseState = statusLineDoing;
			pos = writenum;
			return AGAIN;
		}
		responseState = statusLineDone;
		return AGAIN;
	case statusLineDoing:
		writenum = write(c.cfd, statusLine.c_str() + pos, statusLine.size() - pos);
		c.lastReadTime = std::time(NULL);
#ifdef DEBUG
		std::cout << "-----------------------------------------" << std::endl;
		std::cout << "sending to client (state: statusLineDoing):" << std::endl;
		std::cout << statusLine << std::endl;
		std::cout << "-----------------------------------------" << std::endl;
#endif
		if (writenum == -1)
			return ERROR;
		if (writenum < statusLine.size() - pos)
		{
			pos += writenum;
			return AGAIN;
		}
		pos = 0;
		responseState = statusLineDone;
		return AGAIN;
	case statusLineDone:
		writenum = write(c.cfd, headerOut.c_str(), headerOut.size());
		c.lastReadTime = std::time(NULL);
#ifdef DEBUG
		std::cout << "-----------------------------------------" << std::endl;
		std::cout << "sending to client (state: statusLineDone):" << std::endl;
		std::cout << headerOut << std::endl;
		std::cout << "-----------------------------------------" << std::endl;
#endif
		if (writenum == -1)
			return ERROR;
		if (writenum < headerOut.size())
		{
			responseState = headerOutDoing;
			pos = writenum;
			return AGAIN;
		}
		responseState = headerOutDone;
		return AGAIN;
	case headerOutDoing:
		writenum = write(c.cfd, headerOut.c_str() + pos, headerOut.size() - pos);
		c.lastReadTime = std::time(NULL);
#ifdef DEBUG
		std::cout << "-----------------------------------------" << std::endl;
		std::cout << "sending to client (state: headerOutDoing):" << std::endl;
		std::cout << headerOut << std::endl;
		std::cout << "-----------------------------------------" << std::endl;
#endif
		if (writenum == -1)
			return ERROR;
		if (writenum < headerOut.size() - pos)
		{
			pos += writenum;
			return AGAIN;
		}
		pos = 0;
		responseState = headerOutDone;
		return AGAIN;
	case headerOutDone:
		if (messageBodyOut.size() == 0)
		{
			if (fd_ != -1)
			{
				ssize_t readnum = read(fd_, responseBodyBuf_, sizeof(responseBodyBuf_));
				if (readnum == -1)
				{
					std::cout << "Server Internal Error" << std::endl;
					return ERROR;
				}
				if (readnum == 0)
				{
#ifdef DEBUG
					std::cout << "regular file completely read" << std::endl;
#endif
					close(fd_);
					if (!responseBodyFileName_.empty())
						std::remove(responseBodyFileName_.c_str());
					responseState = responseDone;
					break;
				}
				writenum = write(c.cfd, responseBodyBuf_, readnum);
				c.lastReadTime = std::time(NULL);
#ifdef DEBUG
				std::cout << "-----------------------------------------" << std::endl;
				std::cout << "sending to client from regular file (state: headerOutDone):"
						  << std::endl;
				std::cout << "readnum from regular file: " << readnum << std::endl;
				std::cout << "writenum to client: " << writenum << std::endl;
				// std::cout << "send buf to client: " << responseBodyBuf_ << std::endl;
				std::cout << "-----------------------------------------" << std::endl;
#endif
				if (writenum == -1)
					return ERROR;
				// if (writenum < readnum || writenum == 1024)
				// {
				responseState = messageBodyDoing;
				if (writenum < readnum)
					pos = writenum;
				else
					pos = 1024;
				return AGAIN;
				// }
			}
			else // continue ???
				break;
		}
		else
		{
			writenum = write(c.cfd, messageBodyOut.c_str(), messageBodyOut.size());
			c.lastReadTime = std::time(NULL);
#ifdef DEBUG
			std::cout << "-----------------------------------------" << std::endl;
			std::cout << "sending to client (state: headerOutDone):" << std::endl;
			std::cout << "writenum: " << writenum << std::endl;
			std::cout << "body: " << messageBodyOut << std::endl;
			std::cout << "-----------------------------------------" << std::endl;
#endif
			if (writenum == -1)
				return ERROR;
			if (writenum < messageBodyOut.size())
			{
				responseState = messageBodyDoing;
				pos = writenum;
				return AGAIN;
			}
		}
		responseState = responseDone;
		break;
	case messageBodyDoing:
		if (fd_ != -1)
		{
			if (pos == 1024)
			{
				pos = 0;
				std::memset(responseBodyBuf_, 0, sizeof(responseBodyBuf_));
				ssize_t readnum = read(fd_, responseBodyBuf_, sizeof(responseBodyBuf_));
				if (readnum == -1)
				{
					std::cout << "Server Internal Error" << std::endl;
					return ERROR;
				}
				if (readnum == 0)
				{
#ifdef DEBUG
					std::cout << "regular file completely read" << std::endl;
#endif
					close(fd_);
					if (!responseBodyFileName_.empty())
						std::remove(responseBodyFileName_.c_str());
					responseState = responseDone;
					break;
				}
				writenum = write(c.cfd, responseBodyBuf_, readnum);
				c.lastReadTime = std::time(NULL);
#ifdef DEBUG
				std::cout << "-----------------------------------------" << std::endl;
				std::cout << "sending to client from regular file (state: headerOutDone):"
						  << std::endl;
				std::cout << "readnum from regular file: " << readnum << std::endl;
				std::cout << "writenum to client: " << writenum << std::endl;
				std::cout << "-----------------------------------------" << std::endl;
#endif
				if (writenum == -1)
					return ERROR;
				// if (writenum < readnum || writenum == 1024)
				// {
				if (writenum < readnum)
					pos = writenum;
				else
					pos = 1024;
				return AGAIN;
				// }
			}
			else
			{
				writenum = write(c.cfd, responseBodyBuf_ + pos, sizeof(responseBodyBuf_) - pos);
				c.lastReadTime = std::time(NULL);
#ifdef DEBUG
				std::cout << "-----------------------------------------" << std::endl;
				std::cout << "sending to client from regular file (state: headerOutDone):"
						  << std::endl;
				std::cout << "writenum to client: " << writenum << std::endl;
				std::cout << "-----------------------------------------" << std::endl;
#endif
				if (writenum == -1)
					return ERROR;
				// if (writenum < sizeof(responseBodyBuf_) - pos || pos + writenum == 1024)
				// {
				pos += writenum;
				return AGAIN;
				// }
			}
			// close(fd_);
		}
		else
		{
			writenum = write(c.cfd, messageBodyOut.c_str() + pos, messageBodyOut.size() - pos);
			c.lastReadTime = std::time(NULL);
#ifdef DEBUG
			std::cout << "-----------------------------------------" << std::endl;
			std::cout << "sending to client (state: messageBodyDoing):" << std::endl;
			std::cout << messageBodyOut << std::endl;
			std::cout << "-----------------------------------------" << std::endl;
#endif
			if (writenum == -1)
				return ERROR;
			if (writenum < messageBodyOut.size() - pos)
			{
				pos += writenum;
				return AGAIN;
			}
		}
		responseState = responseDone;
		break;
	}
	if (continueRequest_)
	{
		continueRequest_ = false;
		wevReady = false;
		responseState = 0;
		statusLine = "";
		headerOut = "";
		messageBodyOut = "";
		return processRequest();
	}
	if (ready)
		return readDiscardedRequest();
	return OK;
}

int Http::readDiscardedRequest()
{
#ifdef DEBUG
	std::cout << "readDiscardedRequest" << std::endl;
#endif
	wevReady = false;
	if (alreadyRead)
	{
		alreadyRead = false;
		return AGAIN;
	}
	ssize_t bufSize = 1024;
	char buf[bufSize];
	std::memset(buf, 0, bufSize);
	ssize_t readnum = recv(c.cfd, buf, bufSize, 0);
	if (readnum == -1 || readnum == 0 || readnum < bufSize)
		return OK;
	revHandler = &Http::readDiscardedRequest;
	return AGAIN;
}
