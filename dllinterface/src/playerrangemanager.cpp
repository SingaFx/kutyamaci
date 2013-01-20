#include "playerrangemanager.h"

PlayerRangeManager* PlayerRangeManager::playerRangeManager = 0;

PlayerRangeManager::PlayerRangeManager()
    : playerRanges() 
{
}

PlayerRangeManager::~PlayerRangeManager()
{
}

PlayerRangeManager& PlayerRangeManager::getPlayerRangeManager()
{
    if (!playerRangeManager)
    {
        playerRangeManager = new PlayerRangeManager();
    }

    return *playerRangeManager;
}

void PlayerRangeManager::resetRanges()
{
    playerRanges.clear();

    PlayerRange pr;
    // initialization goes here!!! newly created ranges shoudl be 100% range
    for (int idx = 0; idx < 6; ++idx)
    {
        playerRanges.push_back(pr);
    }

}

PlayerRange PlayerRangeManager::getPlayerRange(int pos)
{
    return playerRanges[pos];
}

void PlayerRangeManager::setPlayerRange(int pos, PlayerRange& pr)
{
    playerRanges[pos] = pr;
}

vector<PlayerRange> PlayerRangeManager::getPlayerRanges()
{
    return playerRanges;
}