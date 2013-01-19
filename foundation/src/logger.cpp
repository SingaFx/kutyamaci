
#include "logger.h"

const string Logger::DLL_INTERFACE_OUTPUT_FILENAME = "dllinterface.log";
const string Logger::HAND_HISTORY_PARSER_OUTPUT_FILENAME = "handhistoryparser.log";

ofstream* Logger::dllInterfaceLogger_ = 0;
ofstream* Logger::handHistoryParserLogger_ = 0;
ofstream* Logger::outfile_ = 0;

Logger* Logger::logger_ = 0;
// --------------------------------------------------
Logger::Logger()
{
    dllInterfaceLogger_ = new ofstream();
    dllInterfaceLogger_->open(DLL_INTERFACE_OUTPUT_FILENAME.c_str());

    handHistoryParserLogger_ = new ofstream();
    handHistoryParserLogger_->open(HAND_HISTORY_PARSER_OUTPUT_FILENAME.c_str());
}

// --------------------------------------------------
Logger::~Logger()
{
    dllInterfaceLogger_->close();
    handHistoryParserLogger_->close();

    delete dllInterfaceLogger_;
    dllInterfaceLogger_ = 0;

    delete handHistoryParserLogger_;
    handHistoryParserLogger_= 0;
}

// --------------------------------------------------
Logger& Logger::getLogger(LOGGER_TYPE lt)
{
	if (!logger_)
	{
		logger_ = new Logger();
	}
   
    switch (lt)
    {
    case DLL_INTERFACE_LOGGER:
        {
            outfile_ = Logger::dllInterfaceLogger_;
        }
        break;
    case HAND_HISTORY_PARSER:
        {
            outfile_ = Logger::handHistoryParserLogger_;
        }
        break;
    };

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