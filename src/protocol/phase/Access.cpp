#include "Access.hpp"

using namespace Wbsv;

int Access::checker(Http& h)
{
	std::cout << "Access checker" << std::endl;
	/* LocationCtx* locationCtx = getLocationCtx(); */
	/* if (locationCtx->limitExcept.size() == 0) */
	return OK;
	/* handler(h); */
}

int Access::handler(Http& h)
{
	/* if ( method is not allowed ) */
	/* return h.finalizeRequest(); */
	/* else */
	return OK;
}
