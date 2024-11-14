// #include "CgiLocationCtx.hpp"
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

/* ---- substitution for test ---- */

// http {
// 	server {
// 		listen localhost:80;
// 		server_name localhost;

// 		location / {
// 			root /root/webserv/test/html;
// 			index index.html;
//			autoindex on;
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
//		error_page 404 /404.html;
//		location /404.html {
//			root /root/webserv/test/html;
//		}
// 	}
// }

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
					if ((hlc = dynamic_cast<HtmlLocationCtx*>(*lit)))
					{
						std::cout << std::setw(20) << std::left << "\t\t\tpath: " << hlc->getPath()
								  << std::endl;
						std::cout << std::setw(20) << std::left << "\t\t\troot: " << hlc->getRoot()
								  << std::endl;
						std::cout << std::setw(20) << std::left
								  << "\t\t\tindex: " << hlc->getIndex() << std::endl;
					}
					else if ((clc = dynamic_cast<CgiLocationCtx*>(*lit)))
					{
						std::cout << std::setw(20) << std::left << "\t\t\tpath: " << clc->getPath()
								  << std::endl;
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
						std::cout << std::setw(20) << std::left << "\t\t\tpath: " << rlc->getPath()
								  << std::endl;
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

// void printHttpConfCtx(HttpConfCtx& httpCtx)
// {
// 	std::cout << "--- HttpConfCtx ---" << std::endl << std::endl;
// 	for (std::vector<ServerCtx>::iterator it = httpCtx.getServerCtxs().begin();
// 		 it != httpCtx.getServerCtxs().end();
// 		 it++)
// 	{
// 		std::cout << "--- Server ---" << std::endl;
// 		std::cout << "Listen IP: " << (*it).getIpAddress() << std::endl;
// 		std::cout << "Listen Port: " << (*it).getPort() << std::endl;
// 		for (std::vector<VServerCtx>::const_iterator ite = (*it).getVServers().begin();
// 			 ite != (*it).getVServers().end();
// 			 ite++)
// 		{

// 			std::cout << std::endl;
// 			std::cout << "--- VServer ---" << std::endl;
// 			std::cout << "isDefaultServer: " << ((*ite).isDefaultServer() ? "true" : "false")
// 					  << std::endl;
// 			std::cout << "Client Max Body Size: " << (*ite).getClientMaxBodySize() << std::endl;
// 			std::cout << "serverNames: ";
// 			for (std::vector<std::string>::const_iterator iter = (*ite).getServerNames().begin();
// 				 iter != (*ite).getServerNames().end();
// 				 iter++)
// 			{
// 				std::cout << (*iter) << " ";
// 			}
// 			std::cout << std::endl;
// 			std::cout << "ErrorPage" << std::endl;
// 			for (std::vector<ErrorPage>::const_iterator iter =
// 					 (*ite).getErrorPages().getErrorPages().begin();
// 				 iter != (*ite).getErrorPages().getErrorPages().end();
// 				 iter++)
// 			{
// 				std::cout << "Page Path: " << (*iter).getErrorPagePath() << std::endl;
// 				std::cout << "Error Numbers: ";
// 				for (std::vector<std::string>::const_iterator itera =
// 						 (*iter).getErrorNumber().begin();
// 					 itera != (*iter).getErrorNumber().end();
// 					 itera++)
// 				{
// 					std::cout << (*itera) << " ";
// 				}
// 				std::cout << std::endl;
// 			}
// 			for (std::vector<LocationCtx*>::const_iterator iter = ite->getLocations().begin();
// 				 iter != ite->getLocations().end();
// 				 iter++)
// 			{
// 				HtmlLocationCtx* html = dynamic_cast<HtmlLocationCtx*>(*iter);
// 				CgiLocationCtx* cgi = dynamic_cast<CgiLocationCtx*>(*iter);
// 				ReturnLocationCtx* retur = dynamic_cast<ReturnLocationCtx*>(*iter);
// 				if (html != NULL)
// 				{
// 					std::cout << "-----HtmlLocationCtx-------" << std::endl;
// 					std::cout << "path: " << html->getPath() << std::endl;
// 					std::cout << "autoIndex: " << html->isAutoIndex() << std::endl;
// 					std::cout << "LimitExcept: " << html->getLimitExcept() << std::endl;
// 					std::cout << "index: " << html->getIndex() << std::endl;
// 					std::cout << "root: " << html->getRoot() << std::endl;
// 				}
// 				if (cgi != NULL)
// 				{
// 					std::cout << "-----CgiLocationCtx-------" << std::endl;
// 					std::cout << "path: " << cgi->getPath() << std::endl;
// 					std::cout << "autoIndex: " << cgi->isAutoIndex() << std::endl;
// 					std::cout << "LimitExcept: " << cgi->getLimitExcept() << std::endl;
// 					std::cout << "CgiIndex: " << cgi->getIndex() << std::endl;
// 					std::cout << "CgiStore: " << cgi->getStore() << std::endl;
// 					std::map<std::string, std::string> param = cgi->getParam();
// 					for (std::map<std::string, std::string>::iterator it = param.begin();
// 						 it != param.end();
// 						 it++)
// 					{
// 						std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
// 					}
// 				}
// 				if (retur != NULL)
// 				{
// 					std::cout << "-----ReturnLocationCtx-------" << std::endl;
// 					std::cout << "path: " << retur->getPath() << std::endl;
// 					std::cout << "autoIndex: " << retur->isAutoIndex() << std::endl;
// 					std::cout << "LimitExcept: " << retur->getLimitExcept() << std::endl;
// 					std::vector<std::string> re = retur->getRedirect();
// 					std::cout << "return: ";
// 					for (std::vector<std::string>::iterator it = re.begin(); it != re.end(); it++)
// 						std::cout << *it << " ";
// 					std::cout << std::endl;
// 				}
// 			}
// 		}
// 		std::cout << "------------------" << std::endl;
// 		std::cout << "------------------" << std::endl << std::endl;
// 	}
// }

/* ----------- end ------------- */

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
		confFileName = "test/conf/test.conf";
	else
		confFileName = argv[1];
	// HttpConfCtx* httpCtx = NULL;
	try
	{
		std::vector<ConfCtx*> ctx;
		// if (argc != 2)
		// 	return 0;
		Wbsv::Webserv ws;
		/* confFileModule */
		ctx = ConfParse::confParse(confFileName);
		// if (!ctx.empty())
		// 	httpCtx = dynamic_cast<HttpConfCtx*>(ctx.back());
		// if (httpCtx != NULL)
		// 	printHttpConfCtx(*httpCtx);
		ws.setConfCtxs(&ctx);
		// printConf(ws.getConfCtxs());
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
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
