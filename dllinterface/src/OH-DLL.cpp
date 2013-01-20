#define WHUSER_EXPORTS 

#include "OH-DLL.h"
#include <windows.h>
#include "logger.h"
#include <string>
#include "currentGameInfo.h"
#include "currentPlayerInfo.h"
#include <algorithm>
#include "gamestatemanager.h"
#include "playerrangemanager.h"
#include "plusEVBotLogic.h"
#include "botmanager.h"
#include "abstractbotlogic.h""

using namespace std;
///////////////////////////////////// 
//card macros 
#define RANK(c)	         ((c>>4)&0x0f) 
#define SUIT(c)          ((c>>0)&0x0f) 
#define ISCARDBACK(c)	 (c==0xff) 
#define ISUNKNOWN(c)	 (c==0) 
///////////////////////////////////// 

//////////////////////////////////// 
//consecutive states 
holdem_state    m_holdem_state[256]; 
unsigned char   m_ndx; 
////////////////////////////////////

//////////////////////////////////// 
//versus list & prwin
phl1k_t m_phl1k;
pp13 prw1326;
////////////////////////////////////

//////////////////////////////////// 
//WH symbols
pfgws_t m_pget_winholdem_symbol;
////////////////////////////////////
static int scrape_cycle = 0;

double gws(int chair, const char* name, bool& iserr)
{ 
	return (*m_pget_winholdem_symbol)(chair,name,iserr); 
} 

double gws(const char* name)
{ 
	bool iserr; 
	int mychair = (int)gws(0,"userchair",iserr); 
	return gws(mychair,name,iserr); 
}

bool hlset( int rank0, int rank1, int listnum, bool onoff )
{
   return ((*m_phl1k)[listnum][rank0-2][rank1-2] = onoff);
}

bool hlget( int rank0, int rank1, int listnum )
{
   return ((*m_phl1k)[listnum][rank0-2][rank1-2]);
}

char convertSuitToChar(int suit)
{
    char result = 'c';

    char map[5];
    map[1] = 'c';
    map[2] = 'd';
    map[3] = 'h';
    map[4] = 's';

    return map[suit];
}

char convertRankToChar(int rank)
{
    char map[15];
    map[2] = '2';
    map[3] = '3';
    map[4] = '4';
    map[5] = '5';
    map[6] = '6';
    map[7] = '7';
    map[8] = '8';
    map[9] = '9';
    map[10] = 'T';
    map[11] = 'J';
    map[12] = 'Q';
    map[13] = 'K';
    map[14] = 'A';

    return map[rank];
}

CurrentGameInfo* createCurrentGameInfo(bool& isValid)
{
    isValid = true; // we are optimistic as always    
    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);
    logger.logExp("CurrentGameInfo : ");

    CurrentGameInfo* currentGameInfo = new CurrentGameInfo();
    // big blind
    double bblind = gws("bblind");
    logger.logExp("[-> bblind] : ", bblind);
    currentGameInfo->setBblind(bblind);

    // hero cards
    double rank1 = gws("$$pr0");
    logger.logExp("-> $$pr0 : ", rank1);

    isValid = isValid && (rank1 >= 2 && rank1 <= 14);

    char crank1 = convertRankToChar((int)rank1);

    double suit1 = gws("$$ps0");    
    logger.logExp("-> $$ps0 : ", suit1);

    char csuit1 = convertSuitToChar((int)suit1);
        
    logger.logExp("-> rank1 : ", crank1);
    logger.logExp("-> suit1 : ", csuit1);

    double rank2 = gws("$$pr1");
    isValid = isValid && (rank1 >= 2 && rank1 <= 14);

    char crank2 = convertRankToChar((int)rank2);
    double suit2 = gws("$$ps1");
    char csuit2 = convertSuitToChar((int)suit2);
    logger.logExp("-> rank2 : ", rank2);
    logger.logExp("-> suit2 : ", suit2);
    
    Card hero1(crank1, csuit1);
    Card hero2(crank2, csuit2);
    Hand holeCard(hero1, hero2);
    currentGameInfo->setHand(holeCard);

    char buffer[100];
    sprintf(buffer,"-> hole cards : %c%c%c%c", crank1, csuit1, crank2, csuit2);
    logger.logExp(buffer);
    // common cards
    vector<Card> board;
    // flop
    double flop1rank = gws("$$cr0");    
    double flop1suit = gws("$$cs0");    

    char f1rank = convertRankToChar((int)flop1rank);
    char f1suit = convertSuitToChar((int)flop1suit);
    Card flop1(f1rank, f1suit);
    board.push_back(flop1);    

    double flop2rank = gws("$$cr1");
    double flop2suit = gws("$$cs1");
    char f2rank = convertRankToChar((int)flop2rank);
    char f2suit = convertSuitToChar((int)flop2suit);
    Card flop2(f2rank, f2suit);
    board.push_back(flop2);

    double flop3rank = gws("$$cr2");
    double flop3suit = gws("$$cs2");
    char f3rank = convertRankToChar((int)flop3rank);
    char f3suit = convertSuitToChar((int)flop3suit);
    Card flop3(f3rank, f3suit);
    board.push_back(flop3);

    // turn
    double turnrank = gws("$$cr3");
    double turnsuit = gws("$$cs3");
    char trank = convertRankToChar((int)turnrank);
    char tsuit = convertSuitToChar((int)turnsuit);
    Card turn(trank, tsuit);
    board.push_back(turn);

    // river
    double riverrank = gws("$$cr4");    
    double riversuit = gws("$$cs4");
    char rrank = convertRankToChar((int)riverrank);
    char rsuit = convertSuitToChar((int)riversuit);
    Card river(rrank, rsuit);
    board.push_back(river);

    currentGameInfo->setBoard(board);

    // betting round
    double bettingRound = gws("br");
    logger.logExp("-> betting round : ", bettingRound);
    currentGameInfo->setStreet(bettingRound-1);

    double amountToCall = gws("call");
    logger.logExp("-> amount to call : ", amountToCall);
    currentGameInfo->setAmountToCall(amountToCall / bblind);

    double potCommon = gws("potcommon");
    logger.logExp("-> pot common : ", potCommon / bblind);
    currentGameInfo->setPotcommon(potCommon);
    
    double totalPot = gws("potplayer");
    logger.logExp("-> total in pot : ", (potCommon + totalPot) / bblind);
    currentGameInfo->setTotalPot(totalPot);


    return currentGameInfo;
}

int nextPosition(int position)
{
    int result = position + 1;
    if (result >= 6)
    {
        result -= 6;
    }

    return result;
}

bool isBitSet(int toTest, int bitNumber)
{
    int bitposition = 1 << bitNumber;
    return (toTest & bitposition) == bitposition;
}

bool isEqual(double d1, double d2)
{
    double eps = 0.001;
    return ( d1 - eps < d2 ) && ( d2 - eps < d1);
}

double getBalanceByPos(int idx)
{    
    char buffer[20];
    sprintf(buffer, "balance%d", idx);
    double balance = gws(buffer);
    return balance;
}

void getCurrentBets(vector<double>& currentBets)
{
    currentBets.clear();

    char buffer[100];
    for (int idx = 0; idx < 6; ++idx)
    {
        sprintf(buffer, "currentbet%d", idx);
        double currentBet = gws(buffer);

        currentBets.push_back(currentBet);
    }
}

void refreshPlayersName(holdem_state* pstate)
{
    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);

    GameStateManager& gameStateManager = GameStateManager::getGameStateManager();

    char buffer[100];
    for (int idx = 0; idx <= 5; ++idx)
    {        
        holdem_player& hp = pstate->m_player[idx];
        
        if (hp.m_name_known & 0x01)
        {
            sprintf(buffer, "holdem_player[%d] -> player name : ", idx);
            logger.logExp(buffer, hp.m_name);

            gameStateManager.setPlayer(string(hp.m_name), idx);
        }
    }
}

void resetHand(holdem_state* pstate, Hand hand)
{
    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);
    logger.logExp("[New hand] : resetting game state!");

    double dealerchair = gws("dealerchair");
    logger.logExp("-> dealerchair : ", dealerchair);

    GameStateManager& gameStateManager = GameStateManager::getGameStateManager();
    gameStateManager.resetState((int)dealerchair, hand);

    // setting up starting balances
    for (int idx = 0; idx < 5; ++idx)
    {        
        gameStateManager.setInitialBalance(idx, getBalanceByPos(idx));
    }
}

void calculateRelativPositions(vector<int>& relativPositions, int dealerPosition)
{    
    int absolutePositionsMap[6];
        
    int pos = dealerPosition;
    for (int idx = 0; idx = 5; ++idx)
    {
        absolutePositionsMap[pos] = idx;
        pos = nextPosition(pos);        
    }

    int relposMap[6];
    relposMap[0] = 0;
    relposMap[1] = 1;
    relposMap[2] = 2;
    relposMap[3] = -3;
    relposMap[4] = -2;
    relposMap[5] = -1;

    relativPositions.clear();
    for (int idx = 0; idx < 6; ++idx)
    {
        int key = absolutePositionsMap[idx];
        relativPositions.push_back(relposMap[key]);
    }
}

void detectMissedCallsAndUpdatePlayerRanges()
{
    GameStateManager& gamestateManager = GameStateManager::getGameStateManager();

    BotManager& botManager = BotManager::getBotManager();
    AbstractBotLogic* botLogic = botManager.getPluggableBot();

    PlayerRangeManager& playerRangeManager = PlayerRangeManager::getPlayerRangeManager();

    double maxRaise = gamestateManager.getMaxRaise();

    double playersplayingbits = gws("playersplayingbits");
    for (int idx = 1; idx < 6; ++idx)
    {        
        if (isBitSet((int)playersplayingbits, idx))
        {
            CurrentPlayerInfo& currentPlayerInfo = gamestateManager.getCurrentPlayerInfo(idx);
            if (!isEqual(maxRaise, currentPlayerInfo.getBetsize()))
            {
                currentPlayerInfo.setLine('c');

                string playerName = currentPlayerInfo.getName();
                PlayerRange& updatedRange = botLogic->calculateRange(playerName, *gamestateManager.getCurrentGameInfo(), playerRangeManager.getPlayerRange(idx));

                playerRangeManager.setPlayerRange(idx, updatedRange);
            }
        }
    }
}

double process_query(const char* pquery)
{
    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);            
    logger.logExp(string("[Processing query] : ").append(pquery).c_str());

	if(pquery == NULL)
    {
		return 0;
    }

    BotManager& botManager = BotManager::getBotManager();
    AbstractBotLogic* botLogic = botManager.getPluggableBot();

    GameStateManager& gamestateManager = GameStateManager::getGameStateManager();
    PlayerRangeManager& playerRangeManager = PlayerRangeManager::getPlayerRangeManager();

    vector<PlayerRange> ranges = playerRangeManager.getPlayerRanges();
    Action action = botLogic->makeDecision(*gamestateManager.getCurrentGameInfo(), ranges);

    if(strcmp(pquery,"dll$swag") == 0)
    {        
		return action.getSize();
    }
    if(strcmp(pquery,"dll$srai")==0)
    {
        return action.getType() == 'r';
    }    
    if(strcmp(pquery,"dll$call")==0)
    {
		return action.getType() == 'c';
    }
    if(strcmp(pquery,"dll$prefold")==0)
    {
		return action.getType() == 'f';
    }

	return 0;

}

double process_state(holdem_state* pstate)
{
    ++scrape_cycle;

    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);
    logger.logExp("[Processing state] : ");

    BotManager& botManager = BotManager::getBotManager();
    AbstractBotLogic* botLogic = botManager.getPluggableBot();

    GameStateManager& gamestateManager = GameStateManager::getGameStateManager();
    PlayerRangeManager& playerRangeManager = PlayerRangeManager::getPlayerRangeManager();

    // first we should test if game state is valid
    bool isValid = true;
    CurrentGameInfo* cgi = createCurrentGameInfo(isValid);
    if (!isValid)
    {
        delete cgi;
        logger.logExp("[SKIPPING SCRAPPING CYCLE] : hero hole cards are not valid!");
        return 0;
    }

    gamestateManager.setCurrentGameInfo(cgi);

    // testing new hand    
    if (gamestateManager.IsHandReset(cgi->getHand()))
    {
        resetHand(pstate, cgi->getHand());
        playerRangeManager.resetRanges();
    }

    // testing if we advanced to the next betting round
    if (cgi->getStreet() > gamestateManager.getBettingRound())
    {
        detectMissedCallsAndUpdatePlayerRanges();
        gamestateManager.resetBettingRound();
    }


    refreshPlayersName(pstate);

    vector<double> currentBets(6);
    getCurrentBets(currentBets);

    vector<int> relativePositions;
    calculateRelativPositions(relativePositions, gamestateManager.getDealerPosition());

    //if (cgi->getStreet() == 0) // preflop
    //{
     //   int smallblindPos = nextPosition(gamestateManager.getDealerPosition());
     //   int bigBlindPos = nextPosition(smallblindPos);
    //}

    double playersplayingbits = gws("playersplayingbits");
    logger.logExp("-> playersplayingbits : ", playersplayingbits);
    for (int idx = 1; idx < 6; ++idx) // hero always sits at 0!
    {
        char buffer[100];            
        if (isBitSet((int)playersplayingbits, idx))
        {
            sprintf_s(buffer, "--> Player %d is playing!",idx);
            logger.logExp(buffer);

            double currentBet = currentBets[idx];

            if (!isEqual(currentBet, gamestateManager.getCurrentBetByPos(idx)))
            {
                if (gamestateManager.isCurrentPlayerInfoSet(idx))
                {
                    CurrentPlayerInfo& currentPlayerInfo = gamestateManager.getCurrentPlayerInfo(idx);

                    currentPlayerInfo.setActualStacksize(getBalanceByPos(idx));
                    currentPlayerInfo.setBetsize(currentBet);

                    double maxRaise = gamestateManager.getMaxRaise();
                    if (isEqual(currentBet, maxRaise))
                    {
                        currentPlayerInfo.setLine('c');
                    }
                    else if (currentBet > maxRaise)
                    {
                        currentPlayerInfo.setLine('r');
                        gamestateManager.setMaxRaise(currentBet);
                    }
                    else
                    {
                        // this is impossible - something went wrong :(
                        logger.logExp("[ERROR at setting last line !]");
                    }

                    // this player did something so we update his/her range
                    string playerName = currentPlayerInfo.getName();
                    PlayerRange playerRange = playerRangeManager.getPlayerRange(idx);
                    //playerRange.set
                    PlayerRange& updatedRange = botLogic->calculateRange(playerName, *cgi, playerRange);

                    playerRangeManager.setPlayerRange(idx, updatedRange);
                }
                else
                {
                    CurrentPlayerInfo currentPlayerInfo;
                        
                    currentPlayerInfo.setStacksize(gamestateManager.getInitialBalanceByPos(idx));
                    currentPlayerInfo.setActualStacksize(getBalanceByPos(idx));
                    currentPlayerInfo.setBetsize(currentBet);
                    currentPlayerInfo.setName(gamestateManager.getPlayerNameByPos(idx));
                    currentPlayerInfo.setPoz(relativePositions[idx]);

                    double maxRaise = gamestateManager.getMaxRaise();
                    if (isEqual(currentBet, maxRaise))
                    {
                        currentPlayerInfo.setLine('c');
                    }
                    else if (currentBet > maxRaise)
                    {
                        currentPlayerInfo.setLine('r');
                        gamestateManager.setMaxRaise(currentBet);
                    }
                    else
                    {
                        // this is impossible - something went wrong :(
                        logger.logExp("[ERROR at setting last line !]");
                    }

                    gamestateManager.setCurrentPlayerInfo(idx, currentPlayerInfo);

                    // let's update
                    string playerName = currentPlayerInfo.getName();
                    PlayerRange& updatedRange = botLogic->calculateRange(playerName, *cgi, playerRangeManager.getPlayerRange(idx));

                    playerRangeManager.setPlayerRange(idx, updatedRange);
                }
            }
        }
        else
        {
            sprintf_s(buffer, "--> Player %d has already folded!",idx);
            logger.logExp(buffer);
        }
    }
               
           
    delete cgi;

	return 0;

}
///////////////////////////////////////////////////// 
//WINHOLDEM RUNTIME ENTRY POINT 
///////////////////////////////////////////////////// 
WHUSER_API double process_message(const char* pmessage, const void* param){

	if (pmessage == NULL)
    {
		return 0;
	}

	if (param == NULL)
    {
		return 0;
	}
	if(strcmp(pmessage,"state") == 0)
    {
		return process_state( (holdem_state*)param );
	}
	if(strcmp(pmessage,"phl1k") == 0){   
		m_phl1k = (phl1k_t)param;
		return 0;
	}
	if(strcmp(pmessage,"prw1326") == 0)
    {
		prw1326 = (pp13)param;
		return 0;
	}
	if(strcmp(pmessage,"query") == 0)
    { 
		return process_query((const char*)param);
	}
	if(strcmp(pmessage,"pfgws") == 0)
    {
		m_pget_winholdem_symbol = (pfgws_t)param;
		return 0;
	}

	return 0;

}
///////////////////////////////// 
//DLLMAIN 
///////////////////////////////// 
BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){

	switch(ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH:
			break; 
		case DLL_THREAD_ATTACH:
			break; 
		case DLL_THREAD_DETACH:
			break; 
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;

}