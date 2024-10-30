#include "Access.hpp"

using namespace Wbsv;

int Access::handler(Http& h)
{
	#ifdef DEBUG
		std::cout << "Access handler" << std::endl;
	#endif
	/* if ( method is not allowed ) */
	/* return h.finalizeRequest(); */
	/* else */
	return OK;
}
