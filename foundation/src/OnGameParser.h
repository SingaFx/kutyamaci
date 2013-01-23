#pragma once

#include "handHistoryParser.h"
#include <string>
#include "HandHistory.h"
#include <vector>

using namespace std;

class OnGameParser : public HandHistoryParser
{
public:
    virtual vector<HandHistory> parse();
    //nice to have: destructor!
};