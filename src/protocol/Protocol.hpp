#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include "Connection.hpp"

namespace Wbsv
{
	class Connection;

	class Protocol
	{
	public:
		virtual ~Protocol(){};
		virtual void revHandler(Connection& c) = 0;
		virtual void wevHandler(Connection& c) = 0;
	};
} // namespace Wbsv

#endif
