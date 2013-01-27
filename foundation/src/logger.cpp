
#include "logger.h"
#include <sstream>
#include <time.h>

const string Logger::DLL_INTERFACE_OUTPUT_FILENAME = "dllinterface.log";
const string Logger::HAND_HISTORY_PARSER_OUTPUT_FILENAME = "handhistoryparser.log";
const string Logger::BOT_LOGIC_OUTPUT_FILENAME = "botlogic.log";
const string Logger::DLL_DECISION_OUTPUT_FILENAME = "dlldecision.log";
const string Logger::RANGE_LOGGER_OUTPUT_FILENAME = "rangeLogger.log";

ofstream* Logger::dllInterfaceLogger_ = 0;
ofstream* Logger::handHistoryParserLogger_ = 0;
ofstream* Logger::botLogicLogger_ = 0;
ofstream* Logger::dllDecisionLogger_ = 0;
ofstream* Logger::rangeLogger_ = 0;
ofstream* Logger::outfile_ = 0;

Logger* Logger::logger_ = 0;
// --------------------------------------------------
Logger::Logger()
{
	srand(time(NULL));
	int random = rand() % 1000000;
	stringstream s;
	s << random;

    dllInterfaceLogger_ = new ofstream();

    dllInterfaceLogger_->open((DLL_INTERFACE_OUTPUT_FILENAME + s.str()).c_str());

    handHistoryParserLogger_ = new ofstream();
	handHistoryParserLogger_->open((HAND_HISTORY_PARSER_OUTPUT_FILENAME + s.str()).c_str());

	botLogicLogger_ = new ofstream();
	botLogicLogger_->open((BOT_LOGIC_OUTPUT_FILENAME.c_str() + s.str()).c_str());

	dllDecisionLogger_ = new ofstream();
	dllDecisionLogger_->open((DLL_DECISION_OUTPUT_FILENAME.c_str() + s.str()).c_str());

	rangeLogger_ = new ofstream();
	rangeLogger_->open((RANGE_LOGGER_OUTPUT_FILENAME.c_str() + s.str()).c_str());
}

// --------------------------------------------------
Logger::~Logger()
{
    dllInterfaceLogger_->close();
    handHistoryParserLogger_->close();
	dllDecisionLogger_->close();

    delete dllInterfaceLogger_;
    dllInterfaceLogger_ = 0;

    delete handHistoryParserLogger_;
    handHistoryParserLogger_= 0;

	delete dllDecisionLogger_;
	dllDecisionLogger_ = 0;
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
	case DLL_DECISION_LOGGER:
		{
			currentStream = dllDecisionLogger_;
		}
		break;
	case RANGE_LOGGER:
		{
			currentStream = rangeLogger_;
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