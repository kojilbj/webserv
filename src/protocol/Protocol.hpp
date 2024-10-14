#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include "ConfFile.hpp"
#include "Connection.hpp"
#include "Listening.hpp"
#include <vector>

namespace Wbsv
{
	class Connection;

	class Protocol
	{
	public:
		virtual ~Protocol(){};
		virtual void revHandler(Connection& c) = 0;
		virtual void getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls) = 0;
	};
} // namespace Wbsv

#endif
