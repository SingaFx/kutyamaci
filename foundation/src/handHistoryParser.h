#pragma once

#include <string>
#include <vector>
#include "HandHistory.h"

using namespace std;

// abstract Parser class, for parsing handhistory files
class HandHistoryParser
{
public:
	virtual vector<HandHistory> parse(string filename) = 0;
    //TODO: Research on virtual constructors. Do not declare it unless you know exactly what are you doing.
};
