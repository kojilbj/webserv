#include "conf/ConfParse.hpp"
#include "core/Webserv.hpp"

/* ---- substitution for test ---- */

/* http { */
/* 	server { */
/* 		listen localhost:80; */
/* 		server_name localhost; */

/* 		location / { */
/* 			root /root/webserv/test/html; */
/* 			index index.html; */
/* 		} */ /* 	} */ /* } */

#include <iostream>

// void Wbsv::confParse(Wbsv::Webserv& ws)
// {
// 	std::vector<ConfCtx*>* confCtxs = new std::vector<ConfCtx*>;
// 	HttpConfCtx* httpConfCtx = new HttpConfCtx;
// 	ServerCtx sc;
// 	sc.addListen("localhost", "80");
// 	VServerCtx vsc;
// 	vsc.defaultServer = true;
// 	vsc.serverName = "localhost";
// 	LocationCtx lc;
// 	lc.path = "/";
// 	lc.root = "/root/webserv/test/html";
// 	lc.index = "index.html";
// 	vsc.addLocationCtx(lc);
// 	sc.addVServerCtx(vsc);
// 	httpConfCtx->addServerCtx(sc);
// 	confCtxs->push_back(httpConfCtx);
// 	ws.setConfCtxs(confCtxs);
// }

// void printConf(std::vector<Wbsv::ConfCtx*>* confCtxs)
// {
// 	std::cout << "--- Parsed Configuration file ---" << std::endl << std::endl;
// 	using namespace Wbsv;
// 	std::cout << "confCtxs size: " << confCtxs->size() << std::endl;
// 	std::vector<ConfCtx*>::iterator it;
// 	for (it = (*confCtxs).begin(); it != (*confCtxs).end(); it++)
// 	{
// 		HttpConfCtx* hcc = (HttpConfCtx*)*it;
// 		std::cout << "mainConf size: " << hcc->getMainCtxs().size() << std::endl;
// 		std::cout << "serverConf size: " << hcc->getServerCtxs().size() << std::endl;
// 		std::vector<ServerCtx>::const_iterator sit;
// 		for (sit = hcc->getServerCtxs().begin(); sit != hcc->getServerCtxs().end(); sit++)
// 		{
// 			std::cout << "\tlisten.first: " << sit->getListen().first << std::endl;
// 			std::cout << "\tlisten.second: " << sit->getListen().second << std::endl;
// 			std::vector<VServerCtx>::const_iterator vsit;
// 			for (vsit = sit->getVServerCtxs().begin(); vsit != sit->getVServerCtxs().end(); vsit++)
// 			{
// 				std::cout << "\tdefaultServer: " << std::boolalpha << vsit->defaultServer
// 						  << std::endl;
// 				std::cout << "\tserver_name: " << vsit->serverName << std::endl;
// 				std::cout << "\tlocationConf size: " << vsit->getLocationCtxs().size() << std::endl;
// 				std::vector<LocationCtx>::const_iterator lit;
// 				for (lit = vsit->getLocationCtxs().begin(); lit != vsit->getLocationCtxs().end();
// 					 lit++)
// 				{
// 					std::cout << "\t\tpath: " << lit->path << std::endl;
// 					std::cout << "\t\troot: " << lit->root << std::endl;
// 					std::cout << "\t\tindex: " << lit->index << std::endl;
// 				}
// 			}
// 		}
// 	}
// 	std::cout << "---------------------------------" << std::endl << std::endl;
// }

void printListening(std::vector<Wbsv::Listening>* lss)
{
	std::cout << "--- Listenings ---" << std::endl << std::endl;
	std::cout << "size: " << lss->size() << std::endl;
	std::cout << "------------------" << std::endl << std::endl;
}

void printHttpConfCtx(HttpConfCtx httpCtx)
{
	std::cout << "--- HttpConfCtx ---" << std::endl << std::endl;
	for (std::vector<ServerCtx>::iterator it = httpCtx.getServerCtxs().begin();
		 it != httpCtx.getServerCtxs().end();
		 it++)
	{
		std::cout << "--- Server ---" << std::endl;
		std::cout << "Listen IP: " << (*it).getIpAddress() << std::endl;
		std::cout << "Listen Port: " << (*it).getPort() << std::endl;
		for (std::vector<VServerCtx>::const_iterator ite = (*it).getVServers().begin();
			 ite != (*it).getVServers().end();
			 ite++)
		{

			std::cout << std::endl;
			std::cout << "--- VServer ---" << std::endl;
			std::cout << "isDefaultServer: " << ((*ite).isDefaultServer() ? "true" : "false")
					  << std::endl;
			std::cout << "Client Max Body Size: " << (*ite).getClientMaxBodySize() << std::endl;
			std::cout << "serverNames: ";
			for (std::vector<std::string>::const_iterator iter = (*ite).getServerNames().begin();
				 iter != (*ite).getServerNames().end();
				 iter++)
			{
				std::cout << (*iter) << " ";
			}
			std::cout << std::endl;
			std::cout << "ErrorPage" << std::endl;
			for (std::vector<ErrorPage>::const_iterator iter =
					 (*ite).getErrorPages().getErrorPages().begin();
				 iter != (*ite).getErrorPages().getErrorPages().end();
				 iter++)
			{
				std::cout << "Page Path: " << (*iter).getErrorPagePath() << std::endl;
				std::cout << "Error Numbers: ";
				for (std::vector<std::string>::const_iterator itera =
						 (*iter).getErrorNumber().begin();
					 itera != (*iter).getErrorNumber().end();
					 itera++)
				{
					std::cout << (*itera) << " ";
				}
				std::cout << std::endl;
			}
			for (std::vector<LocationCtx*>::const_iterator iter = ite->getLocations().begin();
				 iter != ite->getLocations().end();
				 iter++)
			{
				HtmlLocationCtx* html = dynamic_cast<HtmlLocationCtx*>(*iter);
				CgiLocationCtx* cgi = dynamic_cast<CgiLocationCtx*>(*iter);
				ReturnLocationCtx* retur = dynamic_cast<ReturnLocationCtx*>(*iter);
				if (html != nullptr)
				{
					std::cout << "-----HtmlLocationCtx-------" << std::endl;
					std::cout << "path: " << html->getPath() << std::endl;
					std::cout << "autoIndex: " << html->isAutoIndex() << std::endl;
					std::cout << "LimitExcept: " << html->getLimitExcept() << std::endl;
					std::cout << "index: " << html->getIndex() << std::endl;
					std::cout << "root: " << html->getRoot() << std::endl;
				}
				if (cgi != nullptr)
				{
					std::cout << "-----CgiLocationCtx-------" << std::endl;
					std::cout << "path: " << cgi->getPath() << std::endl;
					std::cout << "autoIndex: " << cgi->isAutoIndex() << std::endl;
					std::cout << "LimitExcept: " << cgi->getLimitExcept() << std::endl;
					std::cout << "CgiIndex: " << cgi->getCgiIndex() << std::endl;
					//CgiParam
				}
				if (retur != nullptr)
				{
					std::cout << "-----ReturnLocationCtx-------" << std::endl;
					std::cout << "path: " << retur->getPath() << std::endl;
					std::cout << "autoIndex: " << retur->isAutoIndex() << std::endl;
					std::cout << "LimitExcept: " << retur->getLimitExcept() << std::endl;
					std::vector<std::string> re = retur->getReturn();
					std::cout << "return: ";
					for (std::vector<std::string>::iterator it = re.begin(); it != re.end(); it++)
						std::cout << *it << " ";
					std::cout << std::endl;
				}
			}
		}
		std::cout << "------------------" << std::endl;
		std::cout << "------------------" << std::endl << std::endl;
	}
}

/* ----------- end ------------- */

int main(int argc, char* argv[])
{
	(void)argv;
	if (argc < 2)
		return 0;
	std::string confFileName(argv[1]);
	HttpConfCtx* httpCtx;
	try
	{
		if (argc != 2)
			return 0;
		// Wbsv::Webserv ws;
		/* confFileModule */
		// confParse(ws);
		std::vector<ConfCtx*> ctx = ConfParse::confParse(confFileName);
		if (!ctx.empty())
			httpCtx = dynamic_cast<HttpConfCtx*>(ctx.back());
		if (httpCtx != nullptr)
			printHttpConfCtx(*httpCtx);
		// printConf(ws.getConfCtxs());
		/* initialize 'ws' elements with values got by configuration file */
		/* ex. */
		/* listenings[i].addr = addr; */
		/* listenings[i].port = htons(port); */
		// ws.init();
		// printListening(ws.getListenings());
		/* ws.openListeningSocket(); */
		/* ready for events */
		/* ex. */
		/* epoll_create(); */
		/* epoll_ctl(); */
		/* ev.init(ws); */
		/* ws.processLoop(); */
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
