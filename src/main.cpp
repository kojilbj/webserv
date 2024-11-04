#include "Webserv.hpp"

extern Wbsv::Event* ev;

/* ---- substitution for test ---- */

// http {
// 	server {
// 		listen localhost:80;
// 		server_name localhost;

// 		location / {
// 			root /root/webserv/test/html;
// 			index index.html;
// 		}
// 		location /images/ {
// 			root /root/webserv/test;
// 		}
//		location /redirect {
//			return 301 http://www.google.com;
//		}
//		location /php/ {
//			cgi_index upload.php;
//			cgi_param SCRIPT_FILENAME /root/webserv/test/cgi$cgi_path_info;
//			cgi_store /root/webserv/test/cgi/upload;
//		}
// 	}
// }

#include "CgiLocationCtx.hpp"
#include "HtmlLocationCtx.hpp"
#include "HttpConfCtx.hpp"
#include "LocationCtx.hpp"
#include "ReturnLocationCtx.hpp"
#include "ServerCtx.hpp"
#include "VServerCtx.hpp"
#include <iostream>

void Wbsv::confParse(Wbsv::Webserv& ws)
{
	std::vector<ConfCtx*>* confCtxs = new std::vector<ConfCtx*>;
	HttpConfCtx* httpConfCtx = new HttpConfCtx;
	ServerCtx sc;
	sc.addListen("172.17.0.2", "80");
	VServerCtx vsc;
	vsc.defaultServer = true;
	vsc.serverName = "localhost";
	// this must be dynamicaly allocated
	HtmlLocationCtx* hlc = new HtmlLocationCtx;
	hlc->path = "/";
	hlc->root = "/root/webserv/test/html";
	hlc->index = "index.html";
	vsc.addLocationCtx((LocationCtx*)hlc);
	// this must be dynamicaly allocated
	HtmlLocationCtx* hlc2 = new HtmlLocationCtx;
	hlc2->path = "/images/";
	hlc2->root = "/root/webserv/test";
	vsc.addLocationCtx((LocationCtx*)hlc2);
	// this must be dynamicaly allocated
	ReturnLocationCtx* rlc = new ReturnLocationCtx;
	rlc->path = "/redirect";
	rlc->redirect.first = "301";
	rlc->redirect.second = "http://nginx.org";
	vsc.addLocationCtx((LocationCtx*)rlc);
	// this must be dynamicaly allocated
	CgiLocationCtx* clc = new CgiLocationCtx;
	clc->path = "/php/";
	clc->index = "upload.php";
	clc->param["SCRIPT_FILENAME"] = "/root/webserv/test/cgi$cgi_path_info";
	vsc.addLocationCtx((LocationCtx*)clc);
	sc.addVServerCtx(vsc);
	httpConfCtx->addServerCtx(sc);
	confCtxs->push_back(httpConfCtx);
	ws.setConfCtxs(confCtxs);
	std::vector<LocationCtx*>* v = vsc.getLocationCtxs();
}

void printConf(std::vector<Wbsv::ConfCtx*>* confCtxs)
{
	std::cout << "--- Parsed Configuration file ---" << std::endl << std::endl;
	using namespace Wbsv;
	std::cout << "confCtxs size: " << confCtxs->size() << std::endl;
	std::vector<ConfCtx*>::iterator it;
	for (it = (*confCtxs).begin(); it != (*confCtxs).end(); it++)
	{
		HttpConfCtx* hcc = (HttpConfCtx*)*it;
		std::cout << "mainConf size: " << hcc->getMainCtxs().size() << std::endl;
		std::cout << "serverConf size: " << hcc->getServerCtxs().size() << std::endl;
		std::vector<ServerCtx>::iterator sit;
		for (sit = hcc->getServerCtxs().begin(); sit != hcc->getServerCtxs().end(); sit++)
		{
			std::cout << "\tlisten.first: " << sit->getListen().first << std::endl;
			std::cout << "\tlisten.second: " << sit->getListen().second << std::endl;
			std::vector<VServerCtx>::iterator vsit;
			for (vsit = sit->getVServerCtxs()->begin(); vsit != sit->getVServerCtxs()->end();
				 vsit++)
			{
				std::cout << "\tdefaultServer: " << std::boolalpha << vsit->defaultServer
						  << std::endl;
				std::cout << "\tserver_name: " << vsit->serverName << std::endl;
				std::cout << "\tlocationConf size: " << vsit->getLocationCtxs()->size()
						  << std::endl;
				std::vector<LocationCtx*>::const_iterator lit;
				for (lit = vsit->getLocationCtxs()->begin(); lit != vsit->getLocationCtxs()->end();
					 lit++)
				{
					HtmlLocationCtx* hlc;
					CgiLocationCtx* clc;
					ReturnLocationCtx* rlc;
					if ((hlc = dynamic_cast<HtmlLocationCtx*>(*lit)))
					{
						std::cout << "\t\tpath: " << hlc->path << std::endl;
						std::cout << "\t\troot: " << hlc->root << std::endl;
						std::cout << "\t\tindex: " << hlc->index << std::endl;
					}
					else if ((clc = dynamic_cast<CgiLocationCtx*>(*lit)))
					{
						std::cout << "\t\tpath: " << clc->path << std::endl;
						std::cout << "\t\tSCRIPT_FILENAME: " << clc->param["SCRIPT_FILENAME"]
								  << std::endl;
						std::cout << "\t\tindex: " << clc->index << std::endl;
					}
					else if ((rlc = dynamic_cast<ReturnLocationCtx*>(*lit)))
					{
						std::cout << "\t\tpath: " << rlc->path << std::endl;
						std::cout << "\t\tredirect: " << rlc->redirect.first << " "
								  << rlc->redirect.second << std::endl;
					}
				}
			}
		}
	}
	std::cout << "---------------------------------" << std::endl << std::endl;
}
void printListening(std::vector<Wbsv::Listening>* lss)
{
	std::cout << "--- Listenings ---" << std::endl << std::endl;
	std::cout << "size: " << lss->size() << std::endl;
	std::vector<Wbsv::Listening>::iterator it;
	for (it = lss->begin(); it != lss->end(); it++)
	{
		std::cout << "\tsfd: " << it->sfd << std::endl;
		if (it->family == AF_INET)
			std::cout << "\tfamily: AF_INET" << std::endl;
		if (it->socktype == SOCK_STREAM)
			std::cout << "\tsocktype: SOCK_STREAM" << std::endl;
		char host[NI_MAXHOST];
		char port[NI_MAXSERV];
		getnameinfo((struct sockaddr*)&it->sockaddrIn,
					it->socklen,
					host,
					NI_MAXHOST,
					port,
					NI_MAXSERV,
					NI_NUMERICHOST);
		std::cout << "\thost: " << host << std::endl;
		std::cout << "\tport: " << port << std::endl;
		std::cout << "\tbacklog: " << it->backlog << std::endl;
		std::cout << "\tprotocol: " << it->protocol << std::endl;
	}
	std::cout << "------------------" << std::endl << std::endl;
}

/* ----------- end ------------- */

sig_atomic_t sigInterrupt = 0;

void sigintHandler(int signal)
{
	(void)signal;
	sigInterrupt = 1;
}

int main(int argc, char* argv[])
{
	(void)argv;
	try
	{
		if (argc != 1)
			return 0;
		Wbsv::Webserv ws;
		/* confFileModule */
		confParse(ws);

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
		ev->processEventsLoop(ws);
	}
	catch (std::exception& e)
	{
		e.what();
	}
}
