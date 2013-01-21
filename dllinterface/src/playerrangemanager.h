#pragma once

#include "rangeFunctions.h"
#include "gamestatemanager.h"
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

    void resetRanges(GameStateManager&);

    PlayerRange getPlayerRange(int);
    void setPlayerRange(int pos, PlayerRange&);
	void setPlayerName(int pos, string name);

    vector<PlayerRange> getPlayerRanges();
};