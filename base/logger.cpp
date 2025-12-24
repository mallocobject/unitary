#include "logger.h"
#include "time_stamp.h"
#include <iostream>

namespace unitary
{
Logger& Logger::instance()
{
	static Logger logger;
	return logger;
}

void Logger::log(const std::string& msg)
{
	switch (log_level_)
	{
	case INFO:
		std::cout << "\033[32m[INFO]";
		break;
	case ERROR:
		std::cout << "\033[31m[ERROR]";
		break;
	case FATAL:
		std::cout << "\033[35m[FATAL]";
		break;
	case DEBUG:
		std::cout << "\033[36m[DEBUG]";
		break;
	default:
		break;
	}

	std::cout << TimeStamp::now() << " : " << msg << std::endl;
}

} // namespace unitary