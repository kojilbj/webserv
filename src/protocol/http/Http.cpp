#include "Http.hpp"

using namespace Wbsv;

void Http::revHandler(Connection& c)
{
	HttpRequest hr;
	confCtx = ;
	waitRequestHandler(c);
	processRequestLine(c);
	processRequestHeaders(c);
	wevHandler(c);
}

void Http::wevHandler(Connection&)
{
	coreRunPhase(c);
	finalizeRequest(c);
	finalizeConnection(c);
}

