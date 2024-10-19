#include "Http.hpp"

using namespace Wbsv;

void Http::getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls)
{
	std::vector<ConfCtx*>::iterator it;

	for (it = cfs->begin(); it != cfs->end(); it++)
	{
		ConfCtx* ctx = *it;
		if (ctx->getProtocol() == "HTTP")
		{
			HttpConfCtx* hc = reinterpret_cast<HttpConfCtx*>(ctx);
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
					c.serverCtx = serverCtx;
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

int Http::waitRequestHandler(Connection& c)
{
	char tmp[clientHeaderSize];
	std::memset(tmp, 0, clientHeaderSize);
	ssize_t readnum = recv(c.cfd, tmp, clientHeaderSize, 0);
	if (readnum <= 0)
	{
		// 500 (Internal Server Error) error
		return ERROR;
	}
	std::string buf(tmp, readnum);
#ifndef DEBUG
	std::cout << "after first recv():" << std::endl;
	std::cout << "size: " << std::endl << buf.size() << std::endl;
	std::cout << "buf: " << std::endl << buf << std::endl;
#endif
	std::copy(buf.begin(), buf.end(), std::back_inserter(headerIn));
	int rv = processRequestLine(c);
	if (rv == AGAIN_REQUESTLINE)
	{
		this->revHandler =
			reinterpret_cast<int (Protocol::*)(Connection&)>(&Http::processRequestLine);
		rv = AGAIN;
	}
	else if (rv == AGAIN_REQUESTHEADER)
	{
		this->revHandler =
			reinterpret_cast<int (Protocol::*)(Connection&)>(&Http::processRequestHeader);
		rv = AGAIN;
	}
	return rv;
}

void Http::wevHandler()
{
	/* coreRunPhase(c); */
	/* finalizeRequest(c); */
	/* finalizeConnection(c); */
}

