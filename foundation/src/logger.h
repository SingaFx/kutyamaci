#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
enum LOGGER_TYPE
{
    DLL_INTERFACE_LOGGER = 0,
    HAND_HISTORY_PARSER  = 1,
    BOT_LOGIC            = 2,
	DLL_DECISION_LOGGER  = 3
} ;

class Logger
{
public:

	static Logger& getLogger(LOGGER_TYPE);

	template <class T> void logExp(T exp, LOGGER_TYPE lt)
	{
		outfile_ = getStream(lt);
		(*outfile_) << exp << std::endl;
	}
    /*
	template <class T> void logExp(const char* description, T expression)
	{
		(*outfile_) << description << expression << std::endl;
	}*/
	//void logExp(const char* description, bool expression);
	//void logExp(const char* description, int expression);
	void logExp(const char* description, double expression, LOGGER_TYPE lt)
	{
		outfile_ = getStream(lt);

        (*outfile_) << description << expression << std::endl;
    }

	void logExp(const char* description, const char* expression, LOGGER_TYPE lt)
    {
		outfile_ = getStream(lt);
        (*outfile_) << description << expression << std::endl;
    }

	void logExp(const char* description, int expression, LOGGER_TYPE lt)
    {
		outfile_ = getStream(lt);
        (*outfile_) << description << expression << std::endl;
    }

	void logExp(const char* description, bool expression, LOGGER_TYPE lt)
    {
		outfile_ = getStream(lt);
        (*outfile_) << description << expression << std::endl;
    }

	void logExp(const char* description, char expression, LOGGER_TYPE lt)
    {
		outfile_ = getStream(lt);
        (*outfile_) << description << expression << std::endl;
    }

	ofstream* getStream(LOGGER_TYPE lt);

    static const string DLL_INTERFACE_OUTPUT_FILENAME;
    static const string HAND_HISTORY_PARSER_OUTPUT_FILENAME;
    static const string BOT_LOGIC_OUTPUT_FILENAME;
	static const string DLL_DECISION_OUTPUT_FILENAME;

private:

	Logger();
	~Logger();

	static Logger* logger_;
	
	static ofstream* dllInterfaceLogger_;
    static ofstream* handHistoryParserLogger_;
	static ofstream* botLogicLogger_;
	static ofstream* dllDecisionLogger_;
    static ofstream* outfile_;
};

#endif