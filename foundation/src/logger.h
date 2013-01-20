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
    BOT_LOGIC            = 2
} ;

class Logger
{
public:

	static Logger& getLogger(LOGGER_TYPE);

	template <class T> void logExp(T exp)
	{
		(*outfile_) << exp << std::endl;
	}
    /*
	template <class T> void logExp(const char* description, T expression)
	{
		(*outfile_) << description << expression << std::endl;
	}*/
	//void logExp(const char* description, bool expression);
	//void logExp(const char* description, int expression);
	void logExp(const char* description, double expression)
    {
        (*outfile_) << description << expression << std::endl;
    }
	void logExp(const char* description, const char* expression)
    {
        (*outfile_) << description << expression << std::endl;
    }
	void logExp(const char* description, int expression)
    {
        (*outfile_) << description << expression << std::endl;
    }
	void logExp(const char* description, bool expression)
    {
        (*outfile_) << description << expression << std::endl;
    }
	void logExp(const char* description, char expression)
    {
        (*outfile_) << description << expression << std::endl;
    }

    static const string DLL_INTERFACE_OUTPUT_FILENAME;
    static const string HAND_HISTORY_PARSER_OUTPUT_FILENAME;

private:

	Logger();
	~Logger();

	static Logger* logger_;
	
	static ofstream* dllInterfaceLogger_;
    static ofstream* handHistoryParserLogger_;
    static ofstream* outfile_;
};

#endif