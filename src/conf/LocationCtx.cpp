#include "LocationCtx.hpp"

void LocationCtx::setPath(const std::string& pat)
{
#ifdef DEBUG
	std::cout << "LocationCtx setPath Called" << std::endl;
#endif
	if (pat.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Path: " + pat);
	path = pat;
}

void LocationCtx::setLimitExcept(int auth)
{
#ifdef DEBUG
	std::cout << "LocationCtx setLimitExcept Called" << std::endl;
#endif
	if (auth > GET + DELETE + POST || auth < 0)
		throw std::logic_error("Error Invalid Limit Except: " + ConfParseUtil::intToString(auth));
	limitExcept = auth;
}

void LocationCtx::addLimitExcept(int auth)
{
#ifdef DEBUG
	std::cout << "LocationCtx setLimitExcept Called" << std::endl;
#endif
	if (auth > GET + DELETE + POST || auth < 0)
		throw std::logic_error("Error Invalid Limit Except: " + ConfParseUtil::intToString(auth));
	limitExcept |= auth;
}

void LocationCtx::setLimitExcept(const std::string& auth)
{
#ifdef DEBUG
	std::cout << "LocationCtx setLimitExcept Called" << std::endl;
#endif
	std::vector<std::string> auths;

	if (auth.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Limit Except: " + auth);
	if (auth == "GET")
		limitExcept = GET;
	else if (auth == "POST")
		limitExcept = POST;
	else if (auth == "DELETE")
		limitExcept = DELETE;
	else
		throw std::logic_error("Error Invalid Limit Except: " + auth);
}

void LocationCtx::addLimitExcept(const std::string& auth)
{
#ifdef DEBUG
	std::cout << "LocationCtx setLimitExcept Called" << std::endl;
#endif
	std::vector<std::string> auths;

	if (auth.find(' ') != std::string::npos)
		throw std::logic_error("Error Invalid Limit Except: " + auth);
	if (auth == "GET")
		limitExcept |= GET;
	else if (auth == "POST")
		limitExcept |= POST;
	else if (auth == "DELETE")
		limitExcept |= DELETE;
	else
		throw std::logic_error("Error Invalid Limit Except: " + auth);
}

void LocationCtx::setAutoIndex(bool isOn)
{
#ifdef DEBUG
	std::cout << "LocationCtx setAutoIndex Called" << std::endl;
#endif
	autoIndex = isOn;
}

void LocationCtx::setAutoIndex(const std::string& isOn)
{
#ifdef DEBUG
	std::cout << "LocationCtx setAutoIndex Called" << std::endl;
#endif
	if (isOn == "on")
		autoIndex = true;
	else if (isOn == "off")
		autoIndex = false;
	else
		throw std::logic_error("Error Invalid Auto Index: " + isOn);
}

const std::string& LocationCtx::getPath(void) const
{
	return path;
}

int LocationCtx::getLimitExcept(void) const
{
	return limitExcept;
}

bool LocationCtx::allowGet(void) const
{
	if (limitExcept & GET)
		return true;
	else
		return false;
}

bool LocationCtx::allowPost(void) const
{
	if (limitExcept & POST)
		return true;
	else
		return false;
}

bool LocationCtx::allowDelete(void) const
{
	if (limitExcept & DELETE)
		return true;
	else
		return false;
}

bool LocationCtx::isAutoIndex(void) const
{
	return autoIndex;
}

LocationCtx::LocationCtx(void)
{
#ifdef DEBUG
	std::cout << "Location Constructor Called" << std::endl;
#endif
	limitExcept = GET;
	autoIndex = false;
}

LocationCtx::~LocationCtx(void) { }