#ifndef MODULE_HPP
#define MODULE_HPP

#include "../../core/Webserv.hpp"

namespace Wbsv
{
	class Webserv;

	class Module
	{
	public:
		virtual ~Module(){};
		virtual void init(Webserv& ws) = 0;
		virtual void processEvents(Webserv& ws) = 0;
	};
} // namespace Wbsv

#endif
