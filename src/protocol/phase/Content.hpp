#ifndef CONTENT_HPP
#define CONTENT_HPP

#include "PhaseHandler.hpp"

namespace Wbsv
{
	class Http;

	class Content : public PhaseHandler
	{
	public:
		int handler(Http& h);
	};
} // namespace Wbsv

#include "Http.hpp"

#endif
