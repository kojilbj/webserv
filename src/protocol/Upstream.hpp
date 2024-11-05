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
			: writeFd(0)
			, readFd(0)
			, p(NULL)
			, revHandler_(&Upstream::sendRequestBody)
			, alreadyRead_(false)
			, alreadyWrite_(false)
			, requestBodyFd_(-1)
			, responseBodyFd_(-1)
			, peerClosed(false)
			, lastReadTime(-1)
		{ }
		int invokeRevHandler();
		int writeFd;
		int readFd;
		bool peerClosed;
		Protocol* p;
		time_t lastReadTime;

	private:
		uRevHandler_pt revHandler_;
		int sendRequestBody();
		int recvResponseBody();
		bool alreadyRead_;
		bool alreadyWrite_;
		int requestBodyFd_;
		int responseBodyFd_;
	};
} // namespace Wbsv

#endif