#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
using namespace std;

class Logger
{
public:

	static Logger& getLogger();

	template <class T> void logExp(T exp)
	{
		outfile_ << exp << std::endl;
	}

	template <class T> void logExp(const char* description, T expression)
	{
		outfile_ << description << expression << std::endl;
	}
	//void logExp(const char* description, bool expression);
	//void logExp(const char* description, int expression);
	//void logExp(const char* description, double expression);


private:

	Logger();
	~Logger();

	static Logger* logger_;

	ofstream outfile_;
};

#endif