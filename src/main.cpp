#include "core/Webserv.hpp"
#include <iostream>

#include "core/Webserv.hpp"

void Wbsv::confParse(Wbsv::Webserv& ws)
{
	std::vector<ConfCtx*>* confCtxs = new std::vector<ConfCtx*>;
	HttpConfCtx* httpConfCtx = new HttpConfCtx;
	VServerCtx vsc;
	vsc.listen.first = "localhost";
	vsc.listen.second = "80";
	vsc.server_name = "localhost:80";
	LocationCtx lc;
	lc.path = "/";
	lc.root = "/root/webserv/test/html";
	lc.index = "index.html";
	vsc.locationCtxs.push_back(lc);
	httpConfCtx->vserverCtxs_.push_back(vsc);
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
		std::cout << "mainConf size: " << hcc->mainCtxs_.size() << std::endl;
		std::cout << "vserverConf size: " << hcc->vserverCtxs_.size() << std::endl;
		std::vector<VServerCtx>::iterator vsit;
		for (vsit = hcc->vserverCtxs_.begin(); vsit != hcc->vserverCtxs_.end(); vsit++)
		{
			std::cout << "\tlisten.first: " << vsit->listen.first << std::endl;
			std::cout << "\tlisten.second: " << vsit->listen.second << std::endl;
			std::cout << "\tserver_name: " << vsit->server_name << std::endl;
			std::cout << "\tlocationConf size: " << vsit->locationCtxs.size() << std::endl;
			std::vector<LocationCtx>::iterator lit;
			for (lit = vsit->locationCtxs.begin(); lit != vsit->locationCtxs.end(); lit++)
			{
				std::cout << "\t\tpath: " << lit->path << std::endl;
				std::cout << "\t\troot: " << lit->root << std::endl;
				std::cout << "\t\tindex: " << lit->index << std::endl;
			}
		}
	}
	std::cout << "---------------------------------" << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
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
