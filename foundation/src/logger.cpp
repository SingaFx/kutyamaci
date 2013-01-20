
#include "logger.h"

const string Logger::DLL_INTERFACE_OUTPUT_FILENAME = "dllinterface.log";
const string Logger::HAND_HISTORY_PARSER_OUTPUT_FILENAME = "handhistoryparser.log";
const string Logger::BOT_LOGIC_OUTPUT_FILENAME = "botlogic.log";

ofstream* Logger::dllInterfaceLogger_ = 0;
ofstream* Logger::handHistoryParserLogger_ = 0;
ofstream* Logger::botLogicLogger_ = 0;
ofstream* Logger::outfile_ = 0;

Logger* Logger::logger_ = 0;
// --------------------------------------------------
Logger::Logger()
{
    dllInterfaceLogger_ = new ofstream();
    dllInterfaceLogger_->open(DLL_INTERFACE_OUTPUT_FILENAME.c_str());

    handHistoryParserLogger_ = new ofstream();
    handHistoryParserLogger_->open(HAND_HISTORY_PARSER_OUTPUT_FILENAME.c_str());

	botLogicLogger_ = new ofstream();
	botLogicLogger_->open(BOT_LOGIC_OUTPUT_FILENAME.c_str());
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

	return *logger_;
}

ofstream* Logger::getStream(LOGGER_TYPE lt)
{
	ofstream* currentStream;
	switch(lt)
	{
	case DLL_INTERFACE_LOGGER:
		{
			currentStream = dllInterfaceLogger_;
		}
		break;
	case HAND_HISTORY_PARSER:
		{
			currentStream = handHistoryParserLogger_;
		}
		break;
	case BOT_LOGIC:
		{
			currentStream = botLogicLogger_;
		}
		break;

	}

	return currentStream;
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