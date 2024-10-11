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
	VServerCtx vsc;
	vsc.addListen("localhost", "80");
	vsc.server_name = "localhost:80";
	LocationCtx lc;
	lc.path = "/";
	lc.root = "/root/webserv/test/html";
	lc.index = "index.html";
	vsc.addLocationCtx(lc);
	httpConfCtx->addVServerCtx(vsc);
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
		std::cout << "vserverConf size: " << hcc->getVServerCtxs().size() << std::endl;
		std::vector<VServerCtx>::const_iterator vsit;
		for (vsit = hcc->getVServerCtxs().begin(); vsit != hcc->getVServerCtxs().end(); vsit++)
		{
			std::cout << "\tlisten.first: " << vsit->getListen().first << std::endl;
			std::cout << "\tlisten.second: " << vsit->getListen().second << std::endl;
			std::cout << "\tserver_name: " << vsit->server_name << std::endl;
			std::cout << "\tlocationConf size: " << vsit->getLocationCtxs().size() << std::endl;
			std::vector<LocationCtx>::const_iterator lit;
			for (lit = vsit->getLocationCtxs().begin(); lit != vsit->getLocationCtxs().end(); lit++)
			{
				std::cout << "\t\tpath: " << lit->path << std::endl;
				std::cout << "\t\troot: " << lit->root << std::endl;
				std::cout << "\t\tindex: " << lit->index << std::endl;
			}
		}
	}
	std::cout << "---------------------------------" << std::endl << std::endl;
}

/* ----------- end ------------- */

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

		printConf(ws.getConfCtxs());
		/* initialize 'ws' elements with values got by configuration file */
		/* ex. */
		/* listenings[i].addr = addr; */
		/* listenings[i].port = htons(port); */
		ws.init();
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
