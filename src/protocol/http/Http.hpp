#ifndef HTTP_HPP
#define HTTP_HPP

#include "../../core/ConfFile.hpp"
#include "../../core/Connection.hpp"
#include "../Protocol.hpp"

namespace Wbsv
{
	class HttpConfCtx;
	class Protocol;

	class Http : public Protocol
	{
	public:
		Http(){};
		~Http(){};
		void revHandler(Connection& c);

	private:
		HttpConfCtx* confCtx;
		void wevHandler(Connection& c);
	};

} // namespace Wbsv

#endif
