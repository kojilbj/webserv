#ifndef FINDCONFIG_HPP
#define FINDCONFIG_HPP

#include "PhaseHandler.hpp"

namespace Wbsv
{
	class PhaseHandler;
	class FindConfig : public PhaseHandler
	{
	public:
		int checker(Http& h);
		int handler(Http& h);
	};
} // namespace Wbsv

#endif
