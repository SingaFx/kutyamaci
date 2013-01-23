#pragma once

#include "handHistoryParser.h"
#include <string>
#include "HandHistory.h"
#include <vector>
#include <iostream>

using namespace std;

class BwinPartyParser : public HandHistoryParser
{
private:

    int parserType;
    ifstream& fileHandle;

    void pushActualHandToParseResult(vector<HandHistory>& result, HandHistory& actualhand, int& buttonSeat);
    void clearActualHandInfo(HandHistory& actualhand, int& round);

public:

    BwinPartyParser(int, ifstream&);
    void openFileForParsing(string);
    void closeFileAfterParsing();
    virtual vector<HandHistory> parse();

    void closeFileAfterParsing(string);
    const char* HAND_START;
    const char* HAND_END;
    const char* TABLE;
    const char* PLAYER;
    const char* BBLIND;
    const char* CALL;
    const char* FOLD;
    const char* BET;
    const char* CHECK;
    const char* RAISE;
    const char* ALLIN;
    const char* FLOP;
    const char* TURN;
    const char* RIVER;
    const char* SHOWDOWN;
    const char* BUTTON;
    const char* FILE;
};