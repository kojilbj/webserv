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
		Protocol()
			: wevReady(false){};
		virtual ~Protocol();
		virtual void selectServerCtx(std::vector<ConfCtx*>* cfs, Listening* ls) = 0;
		virtual int invokeRevHandler() = 0;
		virtual void initPhaseHandler() = 0;
		Connection c;
		bool wevReady;

	protected:
		/* const std::string type; */
		ServerCtx* serverCtx;
		std::vector<PhaseHandler*> ph;
	};

} // namespace Wbsv

#include "ConfCtx.hpp"
#include "Listening.hpp"
#include "PhaseHandler.hpp"

#endif
