#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Webserv.hpp"
#include <netdb.h>
#include <string>
#include <sys/socket.h>

namespace Wbsv
{
	class Listening
	{
	public:
		Listening()
			: sfd(-1), result(NULL){};
		~Listening()
		{
			freeaddrinfo(result);
		};
		Listening(const Listening& other)
			: sfd(other.sfd), backlog(other.backlog), protocol(other.protocol)
		{
			result = new struct addrinfo;
			result->ai_flags = other.result->ai_flags;
			result->ai_family = other.result->ai_family;
			result->ai_socktype = other.result->ai_socktype;
			result->ai_protocol = other.result->ai_protocol;
			result->ai_addrlen = other.result->ai_addrlen;
			result->ai_canonname = NULL;
			result->ai_addr = (struct sockaddr_in*)new struct sockaddr_in;
			result->ai_addr->sin_family = other.result->ai_addr->sin_family;
			result->ai_addr->sin_port = other.result->ai_addr->sin_port;
			std::memcpy(&result->ai_addr->sin_addr,
						&other.result->ai_addr->sin_addr,
						sizeof(struct in_addr));
			ls.result->ai_next = NULL;
		}
		int sfd;
		struct addrinfo* result;
		int backlog;
		std::string protocol;
	};

	/* class Connection */
	/* { */
	/* public: */
	/* 	Connection(); */
	/* 	~Connection(); */
	/* 	void setListening(Listening* ls); */
	/* 	void setAcceptRev(int cfd, struct sockaddr_t* sockaddr, socklen_t socklen); */
	/* 	void (*revHandler)(Connection& c); */
	/* 	void (*wevHandler)(Connection& c); */

	/* private: */
	/* 	int cfd; */
	/* 	struct sockaddr_t* remoteSockaddr; */
	/* 	socklen_t remoteSocklen; */
	/* 	Listening* ls; */
	/* 	 arbitrary value (ex. http structure) */
	/* 	Protocol* data; */
	/* } */
} // namespace Wbsv

#endif
