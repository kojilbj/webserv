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
		CgiLocationCtx()
			: LocationCtx()
		{ }
		CgiLocationCtx(const CgiLocationCtx& other)
			: LocationCtx(other), index_(other.index_), param_(other.param_), store_(other.store_)
		{ }
		CgiLocationCtx& operator=(const CgiLocationCtx& other)
		{
			LocationCtx::operator=(other);
			index_ = other.index_;
			param_ = other.param_;
			store_ = other.store_;
			return *this;
		}
		~CgiLocationCtx() { };

		void setCgiIndex(const std::string& cgiIndex);
		void setStore(const std::string& path);
		void setCgiParam(const std::string& key, const std::string& path);
		const std::string& getCgiIndex(void) const;
		const std::map<std::string, std::string>& getCgiParam(void) const;
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