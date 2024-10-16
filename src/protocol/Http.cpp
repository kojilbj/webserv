#include "Http.hpp"

using namespace Wbsv;

void Http::revHandler()
{
	int bufSize = 1024;
	char buf[bufSize];
	std::memset(buf, 0, bufSize);
	for (;;)
	{
/* block at here, when request delay ? */
#ifdef DEBUG
		std::cout << "reading from connection socket (" << c.cfd << ") ..." << std::endl;
#endif
		ssize_t readnum = read(c.cfd, buf, bufSize);
		if (readnum <= 0)
		{
#ifdef DEBUG
			std::cout << "Reading from connection fd (" << c.cfd << ") completed with return value "
					  << readnum << std::endl;
#endif
			break;
		}
#ifdef DEBUG
		std::cout << readnum << " words were read" << std::endl;
		std::cout << "buf:\n" << buf << std::endl;
#endif
		std::memset(buf, 0, bufSize);

		/* char buf2[bufSize]; */
		/* int fd = open("/root/webserv/test/html/index.html", O_RDONLY); */
		/* if (fd < 0) */
		/* { */
		/* 	std::cerr << "failed to open html file" << std::endl; */
		/* 	exit(1); */
		/* } */
		/* for (;;) */
		/* { */
		/* 	std::cout << "entered for(;;)" << std::endl; */
		/* 	if ((readnum = read(fd, buf2, bufSize)) <= 0) */
		/* 		break; */
		/* 	write(c.cfd, buf2, bufSize); */
		/* } */
		/* close(fd); */
	}
	/* HttpRequest hr; */
	/* confCtx = ; */
	/* waitRequestHandler(c); */
	/* processRequestLine(c); */
	/* processRequestHeaders(c); */
	/* wevHandler(c); */
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

void Http::wevHandler(Connection&)
{
	/* coreRunPhase(c); */
	/* finalizeRequest(c); */
	/* finalizeConnection(c); */
}

