#include "gamestatemanager.h"

GameStateManager* GameStateManager::gameStateManager = 0;

GameStateManager::GameStateManager()
    : dealerPosition(-1)
    , bettingRound(-1)
    , lastHand()    
    , currentPlayerInfos(6)
    , isCurrentPlayerInfosSet(6)
    , balance(6)
    , players(6)
    , maxRaiseSize(0.0)
	, currentGameInfo(0)
{
}

GameStateManager::~GameStateManager()
{
}

GameStateManager& GameStateManager::getGameStateManager()
{
    if (!gameStateManager)
    {
        gameStateManager = new GameStateManager();
    }

    return *gameStateManager;
}

int GameStateManager::nextPosition(int pos)
{
    int result = pos + 1;
    if (result >= 6)
    {
        result -= 6;
    }

    return result;
}

bool GameStateManager::IsHandReset(Hand hand)
{
    return lastHand != hand;    
}

void GameStateManager::resetState(int dealerPos, Hand hand)
{
    // reset players
    for (int idx = 0; idx < 6; ++idx)
    {
		players[idx] = string("");
        isCurrentPlayerInfosSet[idx] = false;
        balance[idx] = 0.0;
    }

    // button position
    dealerPosition = dealerPos;

    // reset current bets
    int smallBlindPosition = nextPosition(dealerPosition);
    int bigBlindPosition = nextPosition(smallBlindPosition);

    resetCurrentBets();

    getCurrentPlayerInfo(smallBlindPosition).setBetsize(0.02);       
    getCurrentPlayerInfo(bigBlindPosition).setBetsize(0.04);    
    
    maxRaiseSize = 0.04;
    //
    bettingRound = 0;
    lastHand = hand;
    //     
}

void GameStateManager::resetBettingRound()
{
    maxRaiseSize = 0.0;

    resetCurrentBets();
    nextBettingRound();
}

void GameStateManager::resetCurrentBets()
{
    for (int idx = 0; idx < 6; ++idx)
    {
        CurrentPlayerInfo& cgi = getCurrentPlayerInfo(idx);
        cgi.setBetsize(0.0);          
    }
}

int GameStateManager::getBettingRound()
{
    return bettingRound;
}

void GameStateManager::nextBettingRound()
{
    ++bettingRound;
}

void GameStateManager::setDealerPosition(int bu)
{
    dealerPosition = bu;
}

int GameStateManager::getDealerPosition()
{
    return dealerPosition;
}

void GameStateManager::setHand(Hand hand)
{
    lastHand = hand;
}

Hand GameStateManager::getHand()
{
    return lastHand;
}

double GameStateManager::getCurrentBetByPos(int idx)
{
    CurrentPlayerInfo& cpi = currentPlayerInfos[idx];
    return cpi.getBetsize();
}

void GameStateManager::setCurrentBet(int pos, double currentBet)
{
    CurrentPlayerInfo& cpi = currentPlayerInfos[pos];
    cpi.setBetsize(currentBet);
}

void GameStateManager::setPlayer(string player, int idx)
{
    players[idx] = player;
}

string GameStateManager::getPlayerNameByPos(int idx)
{
    return players[idx];
}

void GameStateManager::setCurrentPlayerInfo(int pos, CurrentPlayerInfo& cpi)
{
    currentPlayerInfos[pos] = cpi;
    isCurrentPlayerInfosSet[pos] = true;
}

CurrentPlayerInfo& GameStateManager::getCurrentPlayerInfo(int pos)
{
    return currentPlayerInfos[pos];
}

bool GameStateManager::isCurrentPlayerInfoSet(int pos)
{
    return isCurrentPlayerInfosSet[pos];
}

double GameStateManager::getInitialBalanceByPos(int pos)
{
    return balance[pos];
}

void GameStateManager::setInitialBalance(int pos, double b)
{
    balance[pos] = b;
}

double GameStateManager::getMaxRaise()
{
    return maxRaiseSize;
}

void GameStateManager::setMaxRaise(double raise)
{
    maxRaiseSize = raise;
}

void GameStateManager::setCurrentGameInfo(CurrentGameInfo* cgi)
{
    currentGameInfo = cgi;
}

CurrentGameInfo* GameStateManager::getCurrentGameInfo()
{
    return currentGameInfo;
}

void GameStateManager::setAction(Action action)
{
	actionCache = action;
}

Action GameStateManager::getAction()
{
	return actionCache;
}

bool GameStateManager::isCacheAvalaible()
{
	return cacheAvalaible;
}

void GameStateManager::setCache(bool cache)
{
	cacheAvalaible = cache;
}