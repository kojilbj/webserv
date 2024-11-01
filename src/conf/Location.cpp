#include "Location.hpp"

Location::Location(void)
{
	std::cout << "Location Constructor Called" << std::endl;
	root_ = "html/";
	index_ = "index.html";
	limit_except_ = GET;
	autoindex_ = false;
}

Location::~Location(void) { }

void Location::setPath(const std::string& path)
{
	std::cout << "location setPath Called" << std::endl;
	path_ = path;
}
void Location::setRoot(const std::string& root)
{
	std::cout << "location setRoot Called" << std::endl;
	root_ = root;
}
void Location::setIndex(const std::string& index)
{
	std::cout << "location setIndex Called" << std::endl;
	index_ = index;
}

void Location::setLimitExcept(int auth)
{
	if (auth > GET + DELETE + POST)
		throw std::exception();
	std::cout << "location setlimitExcept Called" << std::endl;
	limit_except_ = auth;
}
void Location::setAutoIndex(bool isOn)
{
	std::cout << "location setAutoIndex Called" << std::endl;
	autoindex_ = isOn;
}
