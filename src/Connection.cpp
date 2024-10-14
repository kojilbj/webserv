#include "Connection.hpp"

using namespace Wbsv;

void Connection::setListening(Listening* listening)
{
	ls = listening;
}

void Connection::setAcceptRev(int fd, struct sockaddr_in* sockaddrIn, socklen_t socklen)
{
	cfd = fd;
	std::memcpy(&remoteSockaddrIn, sockaddrIn, socklen);
	remoteSocklen = socklen;
}
