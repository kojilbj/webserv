#include "log.hpp"

void printLog(int type, std::string msg)
{
	(void)msg;
	// 2024/01/01 01:01:01
	char now[80];
	std::time_t rawtime = std::time(NULL);
	std::strftime(now, 80, "%Y/%m/%d %T", std::localtime(&rawtime));
	switch (type)
	{
#ifdef DEBUG
	case LOG_DEBUG:
		std::cout << now << " [DEBUG] : " << msg << std::endl;
		break;
#endif
	}
}
