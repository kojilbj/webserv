#include "CgiContent.hpp"

using namespace Wbsv;

int CgiContent::checker(Http& h)
{
	std::cout << "CgiContent checker" << std::endl;
	/* if ( cgi is specified ) */
	/* handler(); */
	/* else */
	return OK;
}

int CgiContent::handler(Http& h)
{
	return OK;
}
