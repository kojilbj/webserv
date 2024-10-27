#include "core/Webserv.hpp"

/* ---- substitution for test ---- */

/* http { */
/* 	server { */
/* 		listen localhost:80; */
/* 		server_name localhost; */

/* 		location / { */
/* 			root /root/webserv/test/html; */
/* 			index index.html; */
/* 		} */
/* 	} */
/* } */

#include <iostream>

void Wbsv::confParse(Wbsv::Webserv& ws)
{
	std::vector<ConfCtx*>* confCtxs = new std::vector<ConfCtx*>;
	HttpConfCtx* httpConfCtx = new HttpConfCtx;
	ServerCtx sc;
	sc.addListen("localhost", "80");
	VServerCtx vsc;
	vsc.defaultServer = true;
	vsc.serverName = "localhost";
	LocationCtx lc;
	lc.path = "/";
	lc.root = "/root/webserv/test/html";
	lc.index = "index.html";
	vsc.addLocationCtx(lc);
	sc.addVServerCtx(vsc);
	httpConfCtx->addServerCtx(sc);
	confCtxs->push_back(httpConfCtx);
	ws.setConfCtxs(confCtxs);
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
		std::vector<ServerCtx>::const_iterator sit;
		for (sit = hcc->getServerCtxs().begin(); sit != hcc->getServerCtxs().end(); sit++)
		{
			std::cout << "\tlisten.first: " << sit->getListen().first << std::endl;
			std::cout << "\tlisten.second: " << sit->getListen().second << std::endl;
			std::vector<VServerCtx>::const_iterator vsit;
			for (vsit = sit->getVServerCtxs().begin(); vsit != sit->getVServerCtxs().end(); vsit++)
			{
				std::cout << "\tdefaultServer: " << std::boolalpha << vsit->defaultServer
						  << std::endl;
				std::cout << "\tserver_name: " << vsit->serverName << std::endl;
				std::cout << "\tlocationConf size: " << vsit->getLocationCtxs().size() << std::endl;
				std::vector<LocationCtx>::const_iterator lit;
				for (lit = vsit->getLocationCtxs().begin(); lit != vsit->getLocationCtxs().end();
					 lit++)
				{
					std::cout << "\t\tpath: " << lit->path << std::endl;
					std::cout << "\t\troot: " << lit->root << std::endl;
					std::cout << "\t\tindex: " << lit->index << std::endl;
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
	std::cout << "------------------" << std::endl << std::endl;
}

/* ----------- end ------------- */

void confParse(std::string& confFileName);

int main(int argc, char* argv[])
{
	(void)argv;
	if (argc < 2)
		return 0;
	std::string confFileName(argv[1]);
	try
	{
		if (argc != 2)
			return 0;
		// Wbsv::Webserv ws;
		/* confFileModule */
		// confParse(ws);
		confParse(confFileName);

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
		e.what();
	}
}
