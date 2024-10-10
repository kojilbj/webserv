#include "Http.hpp"

void httpInitConnection(Webserv::Connection& c)
{
	class Http h;
	c->data = &h;
}
