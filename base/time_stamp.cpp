#include "time_stamp.h"
#include <bits/types/struct_timeval.h>
#include <cinttypes>
#include <cstdint>
#include <ctime>
#include <string>
#include <sys/time.h>

namespace unitary
{
TimeStamp TimeStamp::now()
{
	timeval time;
	::gettimeofday(&time, nullptr);
	int64_t micros = static_cast<int64_t>(time.tv_sec) * 1000000 +
					 static_cast<int64_t>(time.tv_usec);
	return TimeStamp(micros);
}

std::string TimeStamp::toString() const
{
	char buf[32] = {0};
	int64_t seconds = micro_seconds_ / 1000000;
	int64_t microseconds = micro_seconds_ % 1000000;

	tm tm_time;
	localtime_r(&seconds, &tm_time);

	snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
			 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);

	char result[64];
	snprintf(result, sizeof(result), "%s.%06" PRId64, buf, microseconds);
	return result;
}

std::ostream& operator<<(std::ostream& os, const TimeStamp& ts)
{
	os << ts.toString();
	return os;
}

} // namespace unitary