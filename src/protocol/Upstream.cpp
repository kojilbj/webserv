#include "Upstream.hpp"

using namespace Wbsv;

int Upstream::invokeRevHandler()
{
	return (this->*(revHandler_))();
}

int Upstream::sendRequestBody()
{
#ifdef DEBUG
	std::cout << "Upstream::sendRequestBody()" << std::endl;
#endif
	Http* h = reinterpret_cast<Http*>(p);
	if (!alreadyWrite_)
	{
		alreadyWrite_ = true;
		requestBodyFd_ = open(h->requestBodyFileName_.c_str(), O_RDONLY);
		if (requestBodyFd_ == -1)
		{
			revHandler_ = &Upstream::recvResponseBody;
			return OK;
		}
	}
	if (peerClosed)
	{
		close(requestBodyFd_);
		h->wevReady = true;
		int rv = h->createResponse("502");
		if (rv == AGAIN)
			h->revHandler = &Http::coreRunPhase;
		else
			h->revHandler = &Http::finalizeRequest;
		return ERROR;
	}
	int bufSize = 1024;
	char buf[1024];
	std::memset(buf, 0, bufSize);
	ssize_t readnum = read(requestBodyFd_, buf, bufSize);
	if (readnum == -1)
	{
		int rv = h->createResponse("500");
		if (rv == AGAIN)
			h->revHandler = &Http::coreRunPhase;
		else
			h->revHandler = &Http::finalizeRequest;
		return ERROR;
	}
	if (readnum == 0)
	{
		close(requestBodyFd_);
		std::remove(h->requestBodyFileName_.c_str());
		revHandler_ = &Upstream::recvResponseBody;
		return OK;
	}
	ssize_t writenum = write(writeFd, buf, readnum);
	lastReadTime = std::time(NULL);
	if (writenum == -1)
	{
		int rv = h->createResponse("500");
		if (rv == AGAIN)
			h->revHandler = &Http::coreRunPhase;
		else
			h->revHandler = &Http::finalizeRequest;
		return ERROR;
	}
	return AGAIN;
}

int Upstream::recvResponseBody()
{
#ifdef DEBUG
	std::cout << "Upstream::recvResponseBody()" << std::endl;
#endif
	Http* h = reinterpret_cast<Http*>(p);
	if (!alreadyRead_)
	{
		alreadyRead_ = true;
		std::string tmpDir("/tmp/");
		std::string tmpExt(".resBody");
		std::time_t now = std::time(NULL);
		h->responseBodyFileName_ = tmpDir + std::asctime(std::localtime(&now)) + tmpExt;
		responseBodyFd_ = open(h->responseBodyFileName_.c_str(), O_WRONLY | O_CREAT);
		if (responseBodyFd_ == -1)
		{
			h->revHandler = &Http::finalizeRequest;
			return ERROR;
		}
	}
	if (peerClosed)
	{
		close(responseBodyFd_);
		int nextFd = open(h->responseBodyFileName_.c_str(), O_RDONLY);
		h->setFd(nextFd);
		h->wevReady = true;
		h->statusLine = "HTTP/1.1 200 OK\r\n";
		h->revHandler = &Http::finalizeRequest;
		return OK;
	}
	int bufSize = 1024;
	char buf[1024];
	std::memset(buf, 0, bufSize);
	ssize_t readnum = read(readFd, buf, bufSize);
	lastReadTime = std::time(NULL);
	if (readnum == -1)
	{
		int rv = h->createResponse("500");
		if (rv == AGAIN)
			h->revHandler = &Http::coreRunPhase;
		else
			h->revHandler = &Http::finalizeRequest;
		return ERROR;
	}
	if (readnum == 0)
	{
		close(responseBodyFd_);
		int nextFd = open(h->responseBodyFileName_.c_str(), O_RDONLY);
		if (nextFd == -1)
		{
			int rv = h->createResponse("500");
			if (rv == AGAIN)
				h->revHandler = &Http::coreRunPhase;
			else
				h->revHandler = &Http::finalizeRequest;
			return ERROR;
		}
		h->setFd(nextFd);
		h->wevReady = true;
		h->statusLine = "HTTP/1.1 200 OK\r\n";
		h->revHandler = &Http::finalizeRequest;
		return OK;
	}
	ssize_t writenum = write(responseBodyFd_, buf, readnum);
	if (writenum == -1)
	{
		int rv = h->createResponse("500");
		if (rv == AGAIN)
			h->revHandler = &Http::coreRunPhase;
		else
			h->revHandler = &Http::finalizeRequest;
		return ERROR;
	}
	return AGAIN;
}
