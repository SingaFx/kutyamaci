#pragma once

#include "Action.h"
#include "Hand.h"
#include "HandHistory.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class HandHistoryUtils
{
public:

    //checks whether player exists and player history
    static bool playerExists(HandHistory &history, string playername);

    static void addActiontoPlayer(HandHistory &history, Action &action, string playername, int round);

    static void setPlayersHand(HandHistory &history, string playername, Hand &hand);

    static void setPlayersPosition(HandHistory &history, int buttonSeat);

    static void detailedExportToFile(vector<HandHistory> &history, string filename);

    static int exportToFile(vector<HandHistory> &history, string filename);

    static std::vector<HandHistory> importFromFile(FILE *f, int historyNumber);

    static bool isValidHandHistory(HandHistory &history);
};