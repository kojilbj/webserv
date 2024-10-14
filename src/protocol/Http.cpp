#include "Http.hpp"

using namespace Wbsv;

void Http::revHandler(Connection& c)
{
	int bufSize = 1024;
	char buf[bufSize];
	read(c.cfd, buf, bufSize);
	std::cout << buf << std::endl;
	/* HttpRequest hr; */
	/* confCtx = ; */
	/* waitRequestHandler(c); */
	/* processRequestLine(c); */
	/* processRequestHeaders(c); */
	/* wevHandler(c); */
}

void Http::wevHandler(Connection&)
{
	/* coreRunPhase(c); */
	/* finalizeRequest(c); */
	/* finalizeConnection(c); */
}

