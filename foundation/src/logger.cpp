
#include "logger.h"

Logger* Logger::logger_ = 0;
// --------------------------------------------------
Logger::Logger()
{
	outfile_.open("dll_interface.log");
}

// --------------------------------------------------
Logger::~Logger()
{
	outfile_.close();
}

// --------------------------------------------------
Logger& Logger::getLogger()
{
	if (!logger_)
	{
		logger_ = new Logger();
	}

	return *logger_;
}

// --------------------------------------------------
/*
template <class T> void Logger::log(T exp)
{
	outfile_ << exp << std::endl;
}
*/
// --------------------------------------------------
/*
template <class T> void Logger::logExp(const char* description, T expression)
{
	outfile_ << description << exp << std::endl;
}
*/
// --------------------------------------------------
/*
void logExp(const char* description, bool expression)
{
	outfile_ << description << exp << std::endl;
}

// --------------------------------------------------
void logExp(const char* description, int expression)
{
	outfile_ << description << exp << std::endl;
}

// --------------------------------------------------
void logExp(const char* description, double expression)
{
	outfile_ << description << exp << std::endl;
}
*/