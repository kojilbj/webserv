#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <string>

#define GET 1
#define POST 2
#define DELETE 4

class Location
{
public:
	Location(void);
	~Location(void);

	void setPath(const std::string& path);
	void setRoot(const std::string& root);
	void setIndex(const std::string& index);
	void setLimitExcept(int auth);
	void setAutoIndex(bool isOn);

private:
	std::string path_;
	std::string root_;
	std::string index_;
	int limit_except_;
	bool autoindex_;
};

#endif
