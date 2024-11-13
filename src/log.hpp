#ifndef LOG_HPP
#define LOG_HPP

#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

enum
{
	LOG_DEBUG = 0
};

void printLog(int type, std::string msg);

#endif