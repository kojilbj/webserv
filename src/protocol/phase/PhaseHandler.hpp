#ifndef PHASEHANDLER_HPP
#define PHASEHANDLER_HPP

namespace Wbsv
{
	class Http;

	class PhaseHandler
	{
	public:
		virtual int handler(Http&) = 0;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif
