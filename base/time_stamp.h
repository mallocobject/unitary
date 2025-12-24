#ifndef UNITARY_BASE_TIME_STAMP_H
#define UNITARY_BASE_TIME_STAMP_H

#include <cstdint>
#include <string>
namespace unitary
{
class TimeStamp
{
  private:
	int64_t micro_seconds_;

  public:
	TimeStamp() : micro_seconds_(0)
	{
	}

	explicit TimeStamp(int64_t micro_seconds) : micro_seconds_(micro_seconds)
	{
	}
	~TimeStamp()
	{
	}

	static TimeStamp now();
	std::string toString() const;

	friend std::ostream& operator<<(std::ostream& os, const TimeStamp& ts);
};

std::ostream& operator<<(std::ostream& os, const TimeStamp& ts);

} // namespace unitary

#endif