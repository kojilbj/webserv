#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include "Connection.hpp"
#include <string>
#include <vector>

namespace Wbsv
{
	class Connection;
	class Listening;
	class ConfCtx;
	class Protocol;
	class Http;
	class PhaseHandler;

	class Protocol
	{
	public:
		virtual ~Protocol();
		virtual void getServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls) = 0;
		virtual int invokeRevHandler(Connection& c) = 0;
		/* virtual void setRevHandler(revHandler_pt) = 0; */
		virtual void initPhaseHandler() = 0;
		Connection c;

		/* protected: */
	public:
		/* const std::string type; */
		ServerCtx* serverCtx;
		std::vector<PhaseHandler*> ph;
	};

} // namespace Wbsv

#include "ConfFile.hpp"
#include "Listening.hpp"
#include "PhaseHandler.hpp"

#endif
