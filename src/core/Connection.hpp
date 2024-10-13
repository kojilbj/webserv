#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Webserv.hpp"
#include <cstring>
#include <netdb.h>
#include <string>
#include <sys/socket.h>

namespace Wbsv //Core
{
	class Listening
	{
	public:
		Listening();
		~Listening();
		Listening(const Listening& other);
		int sfd;
		int family;
		int socktype;
		struct sockaddr_in sockaddrIn;
		socklen_t socklen;
		int backlog;
		/* Http */
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
