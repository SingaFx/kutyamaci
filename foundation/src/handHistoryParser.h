#pragma once

#include <string>
#include <vector>
#include "HandHistory.h"

using namespace std;

enum PARSER_TYPE
{
    IMPORT_PARSER = 0,
    LIVE_PARSER  = 1
} ;

// abstract Parser class, for parsing handhistory files
class HandHistoryParser
{
public:
	//virtual vector<HandHistory> parse(string filename) = 0;
    virtual vector<HandHistory> parse() = 0;
    //TODO: Research on virtual constructors. Do not declare it unless you know exactly what are you doing.
};
