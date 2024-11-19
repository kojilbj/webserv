#include "Upstream.hpp"

using namespace Wbsv;

int Upstream::invokeRevHandler()
{
	return (this->*(revHandler_))();
}

int Upstream::sendRequestBody()
{
	printLog(LOG_DEBUG, "Upstream::sendRequestBody");
	lastReadTime = std::time(NULL);
	Http* h = reinterpret_cast<Http*>(p);
	if (!alreadyWrite_)
	{
		alreadyWrite_ = true;
		int requestBodyFd = open(h->requestBodyFileName_.c_str(), O_RDONLY);
		if (requestBodyFd == -1)
		{
			revHandler_ = &Upstream::recvResponseBody;
			return OK;
		}
		h->setRequestBodyFileFd(requestBodyFd);
	}
	if (peerClosed)
	{
		h->closeRequestBodyFileFd();
		std::remove(h->requestBodyFileName_.c_str());
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
	ssize_t readnum = read(h->getRequestBodyFileFd(), buf, bufSize);
	if (readnum == -1)
	{
		h->closeRequestBodyFileFd();
		std::remove(h->requestBodyFileName_.c_str());
		h->wevReady = true;
		int rv = h->createResponse("500");
		if (rv == AGAIN)
			h->revHandler = &Http::coreRunPhase;
		else
			h->revHandler = &Http::finalizeRequest;
		return ERROR;
	}
	if (readnum == 0)
	{
		h->closeRequestBodyFileFd();
		std::remove(h->requestBodyFileName_.c_str());
		revHandler_ = &Upstream::recvResponseBody;
		return OK;
	}
	ssize_t writenum = write(writeFd, buf, readnum);
#ifdef DEBUG
	std::stringstream num;
	num << writenum;
	printLog(LOG_DEBUG, num.str() + " byte is written");
#endif
	lastReadTime = std::time(NULL);
	if (writenum == -1)
	{
		h->closeRequestBodyFileFd();
		std::remove(h->requestBodyFileName_.c_str());
		h->wevReady = true;
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
	printLog(LOG_DEBUG, "Upstream::recvResponseBody");
	Http* h = reinterpret_cast<Http*>(p);
	if (!alreadyRead_)
	{
		alreadyRead_ = true;
		std::string tmpDir("/tmp/");
		std::string tmpExt(".resBody");
		std::time_t now = std::time(NULL);
		h->responseBodyFileName_ = tmpDir + std::asctime(std::localtime(&now)) + tmpExt;
		int responseBodyFd =
			open(h->responseBodyFileName_.c_str(), O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
		if (responseBodyFd == -1)
		{
			h->wevReady = true;
			int rv = h->createResponse("500");
			if (rv == AGAIN)
				h->revHandler = &Http::coreRunPhase;
			else
				h->revHandler = &Http::finalizeRequest;
			return ERROR;
		}
		h->setResponseBodyFileFd(responseBodyFd);
	}
	if (peerClosed && !in)
	{
		h->wevReady = true;
		h->closeResponseBodyFileFd();
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
		struct stat st;
		if (stat(h->responseBodyFileName_.c_str(), &st) == -1)
		{
			int rv = h->createResponse("500");
			if (rv == AGAIN)
				h->revHandler = &Http::coreRunPhase;
			else
				h->revHandler = &Http::finalizeRequest;
			return ERROR;
		}
		std::stringstream size;
		size << (st.st_size - headerLen_);
		h->headerOut += "Content-Length: " + size.str() + "\r\n";
		h->setFd(nextFd);
		h->wevReady = true;
		h->statusLine = "HTTP/1.1 200 OK\r\n";
		h->revHandler = &Http::finalizeRequest;
		return OK;
	}
	int bufSize = 1024;
	char buf[bufSize + 1];
	std::memset(buf, 0, bufSize + 1);
	ssize_t readnum = read(readFd, buf, bufSize);
#ifdef DEBUG
	std::stringstream num;
	num << readnum;
	printLog(LOG_DEBUG, num.str() + " byte is read");
#endif
	lastReadTime = std::time(NULL);
	if (readnum == -1)
	{
		h->wevReady = true;
		int rv = h->createResponse("500");
		if (rv == AGAIN)
			h->revHandler = &Http::coreRunPhase;
		else
			h->revHandler = &Http::finalizeRequest;
		return ERROR;
	}
	if (readnum == 0)
	{
		h->wevReady = true;
		h->closeResponseBodyFileFd();
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
		struct stat st;
		if (stat(h->responseBodyFileName_.c_str(), &st) == -1)
		{
			int rv = h->createResponse("500");
			if (rv == AGAIN)
				h->revHandler = &Http::coreRunPhase;
			else
				h->revHandler = &Http::finalizeRequest;
			return ERROR;
		}
		std::stringstream size;
		size << (st.st_size - headerLen_);
		h->headerOut += "Content-Length: " + size.str() + "\r\n";
		h->setFd(nextFd);
		h->statusLine = "HTTP/1.1 200 OK\r\n";
		h->revHandler = &Http::finalizeRequest;
		return OK;
	}
	// assumeing headerLen < 1024 ...
	if (headerLen_ == 0)
	{
		std::string tmp(buf);
		size_t pos = tmp.find("\r\n\r\n");
		if (pos != string::npos)
			headerLen_ = pos + 4;
	}
	ssize_t writenum = write(h->getResponseBodyFileFd(), buf, readnum);
#ifdef DEBUG
	std::stringstream wnum;
	wnum << writenum;
	printLog(LOG_DEBUG, wnum.str() + " byte is written");
#endif
	if (writenum == -1)
	{
		h->wevReady = true;
		int rv = h->createResponse("500");
		if (rv == AGAIN)
			h->revHandler = &Http::coreRunPhase;
		else
			h->revHandler = &Http::finalizeRequest;
		return ERROR;
	}
	return AGAIN;
}
