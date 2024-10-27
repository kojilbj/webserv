#include "Rewrite.hpp"

using namespace Wbsv;

int Rewrite::checker(Http& h)
{
	std::cout << "Rewrite checker" << std::endl;
	/* if ( rewrite is not specified ) */
	return OK;
	/* handler(h); */
}

int Rewrite::handler(Http& h)
{
	/* if ( rewrite destination is like "http://www.google.com" ) */
	/* redirect response */
	/* if ( rewrite destination is like "localhost:80/something" ) */
	/* internal redirect (jump to FindConfig Phase) */
	/* count redirect (redirect loop is up to 5) */
	return OK;
}
