#include "CgiLocationCtx.hpp"
#include "ConfParse.hpp"
#include "HtmlLocationCtx.hpp"
#include "HttpConfCtx.hpp"
#include "LocationCtx.hpp"
#include "ReturnLocationCtx.hpp"
#include "ServerCtx.hpp"
#include "VServerCtx.hpp"
#include "Webserv.hpp"
#include "log.hpp"
#include <iomanip>
#include <iostream>

extern Wbsv::Event* ev;

using namespace Wbsv;

#ifdef DEBUG

void printConf(std::vector<Wbsv::ConfCtx*>* confCtxs)
{
	printLog(LOG_DEBUG, "configuration file parsed");
	std::cout << "\n\n---------------------------------------------\n";
	using namespace Wbsv;
	std::cout << "confCtxs (size: " << confCtxs->size() << "):" << std::endl;
	std::vector<ConfCtx*>::iterator it;
	for (it = (*confCtxs).begin(); it != (*confCtxs).end(); it++)
	{
		HttpConfCtx* hcc = (HttpConfCtx*)*it;
		std::cout << "\tmainConf (size: " << hcc->getMainCtxs().size() << "):" << std::endl;
		std::cout << "\tserverConf (size: " << hcc->getServerCtxs().size() << "):" << std::endl;
		std::vector<ServerCtx>::iterator sit;
		for (sit = hcc->getServerCtxs().begin(); sit != hcc->getServerCtxs().end(); sit++)
		{
			std::cout << std::setw(20) << std::left
					  << "\t\tlisten.first: " << sit->getListen().first << std::endl;
			std::cout << std::setw(20) << std::left
					  << "\t\tlisten.second: " << sit->getListen().second << std::endl;
			std::vector<VServerCtx>::iterator vsit;
			for (vsit = sit->getVServerCtxs()->begin(); vsit != sit->getVServerCtxs()->end();
				 vsit++)
			{
				std::cout << std::setw(20) << std::left << "\n\t\tvserverConf:\n";
				std::cout << std::setw(20) << std::left << "\t\tdefaultServer: " << std::boolalpha
						  << vsit->getDefaultServer() << std::endl;
				std::cout << std::setw(20) << std::left
						  << "\t\tserver_name: " << vsit->getServerNames()[0] << std::endl;
				std::cout << std::setw(20) << std::left
						  << "\t\tclient_max_body_size: " << vsit->getClientMaxBodySize()
						  << std::endl;
				std::cout << std::setw(20) << std::left
						  << "\t\tlocationConf (size: " << vsit->getLocationCtxs()->size()
						  << "):" << std::endl;
				std::vector<LocationCtx*>::const_iterator lit;
				for (lit = vsit->getLocationCtxs()->begin(); lit != vsit->getLocationCtxs()->end();
					 lit++)
				{
					HtmlLocationCtx* hlc;
					CgiLocationCtx* clc;
					ReturnLocationCtx* rlc;
					// std::cout.width(20);
					std::cout << std::setw(20) << std::left << "\t\t\tpath: " << (*lit)->getPath()
							  << std::endl;
					std::cout << std::setw(20) << std::left
							  << "\t\t\tautoindex: " << ((*lit)->isAutoIndex() ? "on" : "off")
							  << std::endl;
					std::cout << std::setw(20) << std::left
							  << "\t\t\tlimit_except: " << ((*lit)->allowGet() ? "GET " : "")
							  << ((*lit)->allowPost() ? "POST " : "")
							  << ((*lit)->allowDelete() ? "DELETE" : "") << std::endl;
					if ((hlc = dynamic_cast<HtmlLocationCtx*>(*lit)))
					{
						std::cout << std::setw(20) << std::left << "\t\t\troot: " << hlc->getRoot()
								  << std::endl;
						std::cout << std::setw(20) << std::left
								  << "\t\t\tindex: " << hlc->getIndex() << std::endl;
					}
					else if ((clc = dynamic_cast<CgiLocationCtx*>(*lit)))
					{
						std::cout << std::setw(20) << std::left
								  << "\t\t\tSCRIPT_FILENAME: " << clc->getParam()["SCRIPT_FILENAME"]
								  << std::endl;
						std::cout << std::setw(20) << std::left
								  << "\t\t\tindex: " << clc->getIndex() << std::endl;
						std::cout << std::setw(20) << std::left
								  << "\t\t\tstore: " << clc->getStore() << std::endl;
					}
					else if ((rlc = dynamic_cast<ReturnLocationCtx*>(*lit)))
					{
						std::cout << std::setw(20) << std::left
								  << "\t\t\tredirect: " << rlc->getRedirect()[0] << " "
								  << rlc->getRedirect()[1] << std::endl;
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
			}
		}
	}
	std::cout << "\n---------------------------------------------\n\n";
}

void printListening(std::vector<Wbsv::Listening>* lss)
{
	printLog(LOG_DEBUG, "listening sockets created");
	std::cout << "\n---------------------------------------------\n\n";
	std::cout << "size: " << lss->size() << std::endl;
	std::vector<Wbsv::Listening>::iterator it;
	for (it = lss->begin(); it != lss->end(); it++)
	{
		std::cout << std::setw(20) << std::left << "\tsfd: " << it->sfd << std::endl;
		if (it->family == AF_INET)
		{
			std::cout << std::setw(20) << std::left << "\tfamily:";
			std::cout << "AF_INET" << std::endl;
		}
		if (it->socktype == SOCK_STREAM)
		{
			std::cout << std::setw(20) << std::left << "\tsocktype:";
			std::cout << "SOCK_STREAM" << std::endl;
		}
		char host[NI_MAXHOST];
		char port[NI_MAXSERV];
		getnameinfo((struct sockaddr*)&it->sockaddrIn,
					it->socklen,
					host,
					NI_MAXHOST,
					port,
					NI_MAXSERV,
					NI_NUMERICHOST);
		std::cout << std::setw(20) << std::left << "\thost: " << host << std::endl;
		std::cout << std::setw(20) << std::left << "\tport: " << port << std::endl;
		std::cout << std::setw(20) << std::left << "\tbacklog: " << it->backlog << std::endl;
		std::cout << std::setw(20) << std::left << "\tprotocol: " << it->protocol << std::endl
				  << std::endl;
	}
	std::cout << "\n---------------------------------------------\n\n";
}

#endif

sig_atomic_t sigInterrupt = 0;

void sigintHandler(int signal)
{
	(void)signal;
	sigInterrupt = 1;
}

int main(int argc, char* argv[])
{
	std::string confFileName;
	if (argc < 2)
		confFileName = "default.conf";
	else
		confFileName = argv[1];
	try
	{
		std::vector<ConfCtx*> ctx;
		Wbsv::Webserv ws;
		/* confFileModule */
		ctx = ConfParse::confParse(confFileName);
		ws.setConfCtxs(&ctx);
		/* initialize 'ws' elements with values got by configuration file */
		/* ex. */
		/* listenings[i].addr = addr; */
		/* listenings[i].port = htons(port); */
		ws.init();
		ws.openListeningSocket();
#ifdef DEBUG
		printConf(ws.getConfCtxs());
		printListening(ws.getListenings());
#endif
		/* ready for events */
		/* ex. */
		/* epoll_create(); */
		/* epoll_ctl(); */
		ev->init(ws);
		signal(SIGINT, sigintHandler);
		signal(SIGPIPE, SIG_IGN);
		ev->processEventsLoop(ws);
	}
	catch (std::string& msg)
	{
		std::cout << "Webserv: " << msg << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "Webserv: " << e.what() << std::endl;
	}
}
