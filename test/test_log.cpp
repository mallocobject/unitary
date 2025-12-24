#include "../base/logger.h"
#include "../base/time_stamp.h"
#include <iostream>

using namespace unitary;

int main()
{
	std::cout << TimeStamp::now().toString() << std::endl;
	std::cout << TimeStamp::now() << std::endl;

	LOG_INFO() << "Hello World!";
	LOG_ERROR() << "Hello World!";
	LOG_FATAL() << "Hello World!";
	LOG_DEBUG() << "Hello World!";
}