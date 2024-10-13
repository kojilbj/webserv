#include "Connection.hpp"

using namespace Wbsv;

Listening::Listening()
	: sfd(-1), family(0), socktype(0), socklen(0), backlog(0)
{
	std::memset(&sockaddrIn, 0, sizeof(struct sockaddr_in));
}

Listening::~Listening() { }

Listening::Listening(const Listening& other)
	: sfd(other.sfd)
	, family(other.family)
	, socktype(other.socktype)
	, socklen(other.socklen)
	, backlog(other.backlog)
	, protocol(other.protocol)
{
	std::memcpy(&sockaddrIn, &other.sockaddrIn, sizeof(struct sockaddr_in));
}

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
