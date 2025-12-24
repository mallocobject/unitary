#ifndef UNITARY_BASE_LOGGER_H
#define UNITARY_BASE_LOGGER_H

#include "common.h"
#include <sstream>
#include <string>

namespace unitary
{
#define LOG(level) Logger::instance().setLogLevel(level)

#define LOG_INFO() LOG(unitary::Logger::INFO)
#define LOG_ERROR() LOG(unitary::Logger::ERROR)
#define LOG_FATAL() LOG(unitary::Logger::FATAL)

#ifndef NODEBUG
#define LOG_DEBUG() LOG(unitary::Logger::DEBUG)
#else
#define LOG_DEBUG()
#endif

class Logger
{
  public:
	enum Level
	{
		INFO,
		ERROR,
		FATAL,
		DEBUG
	};

  private:
	Level log_level_;
	std::ostringstream stream_;
	Logger() = default;

  public:
	DISABLE_COPY_AND_MOVE(Logger)

	static Logger& instance();
	~Logger() = default;

	Logger& setLogLevel(Level level)
	{
		log_level_ = level;
		return *this;
	}

	void log(const std::string& msg);

	template <typename T> Logger& operator<<(const T& val);
};

template <typename T> Logger& Logger::operator<<(const T& val)
{
	stream_ << val;
	log(stream_.str());
	stream_.str("");
	stream_.clear();
	return *this;
}

} // namespace unitary

#endif