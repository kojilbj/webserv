#include "Connection.hpp"

using namespace Wbsv;

Connection::Connection()
	: cfd(-1), upstreamFd(-1), remoteSocklen(0), ls(NULL), lastReadTime(-1)
{
	std::memset(&remoteSockaddrIn, 0, sizeof(struct sockaddr_in));
};

Connection::~Connection() { }

void Connection::setAcceptRev(int fd, struct sockaddr_in* sockaddrIn, socklen_t socklen)
{
	cfd = fd;
	std::memcpy(&remoteSockaddrIn, sockaddrIn, socklen);
	remoteSocklen = socklen;
}
