#pragma once

#include "rangeFunctions.h"
#include <vector>

using namespace std;

class PlayerRangeManager
{
private:
    vector<PlayerRange> playerRanges;

    PlayerRangeManager();
    ~PlayerRangeManager();

    static PlayerRangeManager* playerRangeManager;
public:

    static PlayerRangeManager& getPlayerRangeManager();

    void resetRanges();

    PlayerRange getPlayerRange(int);
    void setPlayerRange(int pos, PlayerRange&);

    vector<PlayerRange> getPlayerRanges();
};