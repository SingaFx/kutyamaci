#pragma once

#include "handHistoryParser.h"
#include <string>
#include "HandHistory.h"
#include <vector>

class BwinPartyParser : public HandHistoryParser
{
private:
    void pushActualHandToParseResult(vector<HandHistory>& result, HandHistory& actualhand, int& buttonSeat);
    void clearActualHandInfo(HandHistory& actualhand, int& round);

public:

    BwinPartyParser();
    virtual vector<HandHistory> parse(string filename);
    const char* HAND_START;
    const char* HAND_END;
    const char* TABLE;
    const char* PLAYER;
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