#ifndef UPSTREAM_HPP
#define UPSTREAM_HPP

#include <Webserv.hpp>
#include <iostream>

namespace Wbsv
{
	class Upstream;

	typedef int (Upstream::*uRevHandler_pt)();

	class Upstream
	{
	public:
		Upstream()
			: writeFd(0), readFd(0), p(NULL), revHandler_(&Upstream::sendRequestBody)
		{ }
		int invokeRevHandler();
		int writeFd;
		int readFd;
		Protocol* p;

	private:
		uRevHandler_pt revHandler_;
		int sendRequestBody();
		int recvResponseBody();
	};
} // namespace Wbsv

#endif