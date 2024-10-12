#include "Connection.hpp"

using namespace Wbsv;

Listening::Listening()
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
