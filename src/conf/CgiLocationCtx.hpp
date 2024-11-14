#ifndef CGILOCATIONCTX_HPP
#define CGILOCATIONCTX_HPP

#include "LocationCtx.hpp"
#include <cstdio>
#include <cstring>
#include <map>
#include <unistd.h>

namespace Wbsv
{
	class Http;

	class CgiLocationCtx : public LocationCtx
	{
	public:
		CgiLocationCtx();
		CgiLocationCtx(const CgiLocationCtx& other);
		CgiLocationCtx& operator=(const CgiLocationCtx& other);
		~CgiLocationCtx();

		void setIndex(const std::string& cgiIndex);
		void setStore(const std::string& path);
		void setParam(const std::string& key, const std::string& path);
		const std::string& getIndex(void) const;
		std::map<std::string, std::string>& getParam(void);
		const std::string& getStore(void) const;
		int contentHandler(Http& h);

	private:
		std::string index_;
		std::map<std::string, std::string> param_;
		std::string store_;
	};
} // namespace Wbsv

#include "Http.hpp"

#endif