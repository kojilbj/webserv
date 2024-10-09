#include "Webserv.hpp"

int main(int argc, char* argv[])
{
	try
	{
		Webserv::Webserv ws;
		/* confFileModule */
		Webserv::confParse(ws.confCtxs, argv[1]);
		/* initialize 'ws' elements with values got by configuration file */
		/* ex. */
		/* listenings[i].addr = addr; */
		/* listenings[i].port = htons(port); */
		ws.init();
		ws.openListeningSocket();
		/* ready for events */
		/* ex. */
		/* epoll_create(); */
		/* epoll_ctl(); */
		ev.init(ws);
		ws.processCycle();
	}
	catch (std::exception& e)
	{
		e.what();
	}
}
