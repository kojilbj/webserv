#ifndef CGICONTENT_HPP
#define CGICONTENT_HPP

#include "PhaseHandler.hpp"

namespace Wbsv
{
	class CgiContent : public PhaseHandler
	{
	public:
		int checker(Http& h);
		int handler(Http& h);
	};
} // namespace Wbsv

#endif
