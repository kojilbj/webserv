#ifndef MODULE_HPP
#define MODULE_HPP

#include "../../core/Webserv.hpp"

namespace Webserv
{
	class Webserv;

	class Module
	{
	public:
		virtual ~Module(){};
		virtual void init(Webserv& ws) = 0;
		/* virtual processEvents(Webserv& ws) = 0; */
	};
} // namespace Webserv

#endif
