#include "Http.hpp"

using namespace Wbsv;

void Http::processRequestLine(HttpRequest& r)
{
	size_t bufSize = 1024;
	char buf[bufSize];
	ssize_t readnum;

	/* client_header_buffer_size 1k; */
	/* large_client_header_buffers 4 8k; */
	for (;;)
	{
		std::memset(buf, 0, bufSize);
		readnum = recv(c.cfd, buf, bufSize);
		if (readnum < 0)
		{
			std::cerr << "recv failed: unknown error" << std::endl;
			exit(1);
		}
		if (readnum >= bufSize)
			readnum = recv(c.cfd, buf, bufSize);
	}
}

void Http::revHandler()
{
	HttpRequest r;
	processRequestLine();
	processRequestHeaders();
	wevHandler();
	/* int bufSize = 1024; */
	/* char buf[bufSize]; */
	/* std::memset(buf, 0, bufSize); */
	/* for (;;) */
	/* { */
	/* /1* block at here, when request delay ? *1/ */
	/* #ifdef DEBUG */
	/* 	std::cout << "reading from connection socket (" << c.cfd << ") ..." << std::endl; */
	/* #endif */
	/* 	ssize_t readnum = read(c.cfd, buf, bufSize); */
	/* 	if (readnum <= 0) */
	/* 	{ */
	/* #ifdef DEBUG */
	/* 		std::cout << "Reading from connection fd (" << c.cfd << ") completed with return value " */
	/* 				  << readnum << std::endl; */
	/* #endif */
	/* 		break; */
	/* 	} */
	/* #ifdef DEBUG */
	/* 	std::cout << readnum << " words were read" << std::endl; */
	/* 	std::cout << "buf:\n" << buf << std::endl; */
	/* #endif */
	/* 	std::memset(buf, 0, bufSize); */

	/* 	/1* char buf2[bufSize]; *1/ */
	/* 	/1* int fd = open("/root/webserv/test/html/index.html", O_RDONLY); *1/ */
	/* 	/1* if (fd < 0) *1/ */
	/* 	/1* { *1/ */
	/* 	/1* 	std::cerr << "failed to open html file" << std::endl; *1/ */
	/* 	/1* 	exit(1); *1/ */
	/* 	/1* } *1/ */
	/* 	/1* for (;;) *1/ */
	/* 	/1* { *1/ */
	/* 	/1* 	std::cout << "entered for(;;)" << std::endl; *1/ */
	/* 	/1* 	if ((readnum = read(fd, buf2, bufSize)) <= 0) *1/ */
	/* 	/1* 		break; *1/ */
	/* 	/1* 	write(c.cfd, buf2, bufSize); *1/ */
	/* 	/1* } *1/ */
	/* 	/1* close(fd); *1/ */
	/* } */
}

void Http::getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls)
{
	std::vector<ConfCtx*>::iterator it;

	for (it = cfs->begin(); it != cfs->end(); it++)
	{
		ConfCtx* c = *it;
		if (c->getProtocol() == "HTTP")
		{
			HttpConfCtx* hc = reinterpret_cast<HttpConfCtx*>(c);
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
				std::pair<std::string, std::string> listen = sit->getListen();
				if (getaddrinfo(listen.first.c_str(), listen.second.c_str(), &hints, &result) != 0)
				{
					std::cerr << strerror(errno) << std::endl;
					exit(1);
				}
				struct sockaddr_in* addrIn = (struct sockaddr_in*)result->ai_addr;
				if (addrIn->sin_port == ls->sockaddrIn.sin_port &&
					addrIn->sin_addr.s_addr == ls->sockaddrIn.sin_addr.s_addr)
				{
					serverCtx = &(*sit);
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
}

void Http::wevHandler()
{
	/* coreRunPhase(c); */
	/* finalizeRequest(c); */
	/* finalizeConnection(c); */
}

