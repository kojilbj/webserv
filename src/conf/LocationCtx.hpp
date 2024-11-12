#ifndef LOCATIONCTX_HPP
#define LOCATIONCTX_HPP

#include <iostream>
#include <string>

#include "ConfParseUtil.hpp"

#define GET 1
#define POST 2
#define DELETE 4

class LocationCtx
{
public:
	LocationCtx(void);
	virtual ~LocationCtx(void) = 0;

	virtual void contentHandler(void) = 0;

	void setPath(const std::string& path);
	void setLimitExcept(int auth);
	void setLimitExcept(const std::string& auth);
	void addLimitExcept(int auth);
	void addLimitExcept(const std::string& auth);
	void setAutoIndex(bool isOn);
	void setAutoIndex(const std::string& autoIndex);

	const std::string& getPath(void) const;
	int getLimitExcept(void) const;
	bool allowGet(void) const;
	bool allowPost(void) const;
	bool allowDelete(void) const;
	bool isAutoIndex(void) const;

protected:
	std::string path;
	int limitExcept;
	bool autoIndex;
};

#endif