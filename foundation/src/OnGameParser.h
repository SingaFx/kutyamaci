#pragma once

#include "handHistoryParser.h"
#include <string>
#include "HandHistory.h"
#include <vector>

using namespace std;

class OnGameParser
{
public:
    vector<HandHistory> parse(string);
    //nice to have: destructor!
};