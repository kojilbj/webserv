#include "Upstream.hpp"

using namespace Wbsv;

int Upstream::invokeRevHandler()
{
	return (this->*(revHandler_))();
}

int Upstream::sendRequestBody()
{
#ifndef DEBUG
	std::cout << "Upstream::sendRequestBody()" << std::endl;
#endif
	Http* h = reinterpret_cast<Http*>(p);
	struct stat st;
	if (stat(h->requestBodyFileName_.c_str(), &st) == -1)
	{
		// Server Internal Error
		h->revHandler = &Http::finalizeRequest;
		return ERROR;
	}
	// maybe too much ?
	int bodyLen = static_cast<int>(st.st_size);
	int fd = open(h->requestBodyFileName_.c_str(), O_RDONLY);
	char buf[bodyLen];
	ssize_t readnum = read(fd, buf, bodyLen);
	ssize_t writenum = write(writeFd, buf, readnum);
	revHandler_ = &Upstream::recvResponseBody;
	return OK;
	// if (alreadyRead)
	// {
	// 	pos = 1024;
	// 	std::string tmpDir("/tmp/");
	// 	std::string tmpExt(".resBody");
	// 	std::time_t now = std::time(NULL);
	// 	responseBodyFileName_ = tmpDir + std::asctime(std::localtime(&now)) + tmpExt;
	// 	responseBodyFileFd_ = open(responseBodyFileName_.c_str(), O_WRONLY | O_CREAT);
	// 	if (responseBodyFileFd_ == -1)
	// 	{
	// 		// Server Internal Error
	// 		return finalizeRequest();
	// 	}
	// 	alreadyRead = false;
	// 	return UPSTREAM_AGAIN_FIRST;
	// }
	// else
	// {
	// 	if (pos == 1024)
	// 	{
	// 		pos = 0;
	// 		std::memset(responseBodyBuf_, 0, sizeof(responseBodyBuf_));
	// 		ssize_t readnum = read(c.upstreamFd, responseBodyBuf_, sizeof(responseBodyBuf_));
	// 		std::cout << "readnum: " << readnum << std::endl;
	// 		if (readnum < 0)
	// 			return ERROR;
	// 		if (readnum == 0)
	// 		{
	// 			close(c.upstreamFd);
	// 			close(responseBodyFileFd_);
	// 			fd_ = open(responseBodyFileName_.c_str(), O_RDONLY);
	// 			if (fd_ == -1)
	// 			{
	// 				// Server Internal Error
	// 				return finalizeRequest();
	// 			}
	// 			// waitpid();
	// 			statusLine = "HTTP/1.1 200 OK\r\n";
	// 			return finalizeRequest();
	// 		}
	// 		ssize_t writenum = write(responseBodyFileFd_, responseBodyBuf_, readnum);
	// 		if (writenum == -1)
	// 		{
	// 			// Server Internal Error
	// 			return finalizeRequest();
	// 		}
	// 		if (writenum < readnum)
	// 			pos += writenum;
	// 		else
	// 			pos = 1024;
	// 		return UPSTREAM_AGAIN;
	// 	}
	// 	else
	// 	{
	// 		ssize_t writenum =
	// 			write(responseBodyFileFd_, responseBodyBuf_ + pos, sizeof(responseBodyBuf_) - pos);
	// 		if (writenum == -1)
	// 		{
	// 			// Server Internal Error
	// 			return finalizeRequest();
	// 		}
	// 		if (writenum < sizeof(responseBodyBuf_) - pos)
	// 			pos += writenum;
	// 		else
	// 			pos = 1024;
	// 		return UPSTREAM_AGAIN;
	// 	}
	// }
}

int Upstream::recvResponseBody()
{
#ifdef DEBUG
	std::cout << "Upstream::recvResponseBody()" << std::endl;
#endif
	Http* h = reinterpret_cast<Http*>(p);
	int fd = open(h->responseBodyFileName_.c_str(), O_WRONLY | O_CREAT);
	size_t bufSize = 1024;
	char buf[bufSize];
	ssize_t readnum = read(readFd, buf, bufSize);
	ssize_t writenum = write(writeFd, buf, readnum);
	close(fd);
	int nextFd = open(h->responseBodyFileName_.c_str(), O_RDONLY);
	h->setFd(nextFd);
	h->revHandler = &Http::finalizeRequest;
	return OK;
}