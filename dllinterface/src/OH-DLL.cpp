#define WHUSER_EXPORTS 

#include "windows.h"
//#include "stdafx.h"

#include "OH-DLL.h"
#include "logger.h"
#include <string>
#include "currentGameInfo.h"
#include "currentPlayerInfo.h"
#include <algorithm>
#include "gamestatemanager.h"
#include "playerrangemanager.h"
#include "plusEVBotLogic.h"
#include "botmanager.h"
#include "abstractbotlogic.h"
#include "database.h"
#include <sstream>


using namespace std;

#define BBLIND (GameStateManager::getGameStateManager().getCurrentGameInfo()->getBblind())


#define MYMENU_EXIT         (WM_APP + 101)
#define MYMENU_MESSAGEBOX   (WM_APP + 102) 

HINSTANCE  inj_hModule;          //Injected Modules Handle
HWND       prnt_hWnd;            //Parent Window Handle
HWND hwnd;

vector<string> myOutput;

void getCurrentBets(vector<double>& currentbets, double bblind);

void WriteToDebugWindow()
{
	Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);
   
	stringstream stream;

	GameStateManager& gamestate = GameStateManager::getGameStateManager();
	CurrentGameInfo* cgi = gamestate.getCurrentGameInfo();

	if (cgi == NULL) 
	{
		myOutput.push_back("cgi == NULL");
		RedrawWindow(hwnd , NULL , NULL , RDW_INVALIDATE);
		return ;
	}

	//POT
	/*
	double totalpot = cgi->getTotalPot();
	stream << totalpot;

	myOutput.push_back("Totalpot = " + stream.str());
	logger.logExp("Totalpot = " + stream.str(), DLL_INTERFACE_LOGGER);
	*/

	/*
	double newHandNumber = cgi->getHandNumber();
	double oldHandNumber = gamestate.getHandNumber();
	stream.clear();
	stream << "Hand numbers: " << newHandNumber << " " << oldHandNumber;
	myOutput.push_back(stream.str());
	*/

	//USERS
	/*
	for (int i = 0; i < 6; ++i)
	{
		string s = gamestate.getPlayerNameByPos(i);
		myOutput.push_back(s);
	}

	for (int i = 0; i < 6; ++i)
	{
		if (!gamestate.isCurrentPlayerInfoSet(i)) continue;
		string s = gamestate.getCurrentPlayerInfo(i).getName();
		myOutput.push_back(s);
	}
	*/


	/*
	stream.clear();
	double betround1 = gamestate.getBettingRound();
	double betround2 = cgi->getStreet();

	stream << "Betround= " << betround1 << " " << betround2;
	myOutput.push_back(stream.str());
	*/

	//BETROUND
	{
		ostringstream os;

		os << "BETROUND: " << gamestate.getBettingRound();
		myOutput.push_back(os.str());
	}

	{
		ostringstream os;

		os << "MaxRaise: " << gamestate.getMaxRaise();
		myOutput.push_back(os.str());
	}

	{
		ostringstream os;

		os << "PotCommon: " << gamestate.getCurrentGameInfo()->getPotcommon();
		myOutput.push_back(os.str());
	}

	{
		ostringstream os;

		os << "FLop pot common: " << gamestate.getCurrentGameInfo()->getFlopPotSize();
		myOutput.push_back(os.str());
	}

	stream.clear();

	vector<double> currentBets(6);
	getCurrentBets(currentBets, cgi->getBblind());

	for (int i = 0; i < currentBets.size(); ++i)
	{
		stream << currentBets[i] << " ";
	}
	stream << endl;
	myOutput.push_back(stream.str());

	//stream.clear();
	

	for (int idx = 0; idx < 6; ++idx)
	{
		if (gamestate.isCurrentPlayerInfoSet(idx))
		{
			ostringstream os;
			string name = gamestate.getCurrentPlayerInfo(idx).getName();
			double line = gamestate.getCurrentPlayerInfo(idx).getLine();
			double betsize = gamestate.getCurrentPlayerInfo(idx).getBetsize();
			double betsize2 = gamestate.getCurrentBetByPos(idx);
			double bblind = cgi->getBblind();
			double stacksize = gamestate.getCurrentPlayerInfo(idx).getStacksize();

			os << name << " " << line << " " << betsize << " " << betsize2 << " " << bblind << " " << stacksize;
			myOutput.push_back(os.str());
		}
	}

	for (int idx = 0; idx < 6; ++idx)
	{
		PlayerRange range = PlayerRangeManager::getPlayerRangeManager().getPlayerRange(idx);

		ostringstream os;
		os << gamestate.getPlayerNameByPos(range.getId()) << " " << range.totalPercentage();
		myOutput.push_back(os.str());
	}

	/*
	stream.clear();
	int random = rand() % 1000;
	stream << random;
	myOutput.push_back("random= " + stream.str());
	*/

	/*
	stream.clear();
	random = rand() % 1000;
	stream << random;
	myOutput.push_back("random= " + stream.str());
	*/
	//myOutput.clear();
	RedrawWindow(hwnd , NULL , NULL , RDW_INVALIDATE);
}

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

int nextPosition(int position)
{
    int result = position + 1;
    if (result >= 6)
    {
        result -= 6;
    }

    return result;
}

void calculateRelativPositions(vector<int>& relativPositions, int dealerPosition, bool usePostFlopRelatives = false)
{    
    int absolutePositionsMap[6];
        
    int pos = dealerPosition;
    for (int idx = 0; idx <= 5; ++idx)
    {
        absolutePositionsMap[pos] = idx;
        pos = nextPosition(pos);        
    }

    int relposMap[6];
    if (!usePostFlopRelatives)
    {
        relposMap[0] = 0;
        relposMap[1] = 1;
        relposMap[2] = 2;
        relposMap[3] = -3;
        relposMap[4] = -2;
        relposMap[5] = -1;
    }
    else
    {
        relposMap[0] = 0; // bu
        relposMap[1] = -5; // sb
        relposMap[2] = -4; // bb
        relposMap[3] = -3; // utg
        relposMap[4] = -2; // mp
        relposMap[5] = -1; // guess what : co
    }

    relativPositions.clear();
    for (int idx = 0; idx < 6; ++idx)
    {
        int key = absolutePositionsMap[idx];
        relativPositions.push_back(relposMap[key]);
    }
}

CurrentGameInfo* createCurrentGameInfo(bool& isValid)
{
    isValid = true; // we are optimistic as always    
    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);
    logger.logExp("CurrentGameInfo : ", DLL_INTERFACE_LOGGER);

    CurrentGameInfo* currentGameInfo = new CurrentGameInfo();

	double handNumber = gws("handsplayed");
	logger.logExp("[-> hand number] : ", handNumber, DLL_INTERFACE_LOGGER);
	currentGameInfo->setHandNumber(handNumber);

    // big blind
    double bblind = gws("bblind");
    logger.logExp("[-> bblind] : ", bblind, DLL_INTERFACE_LOGGER);
    currentGameInfo->setBblind(bblind);

	// betting round
    double bettingRound = gws("br");
    logger.logExp("-> betting round : ", bettingRound, DLL_INTERFACE_LOGGER);
    currentGameInfo->setStreet(bettingRound-1);

    // hero cards
    double rank1 = gws("$$pr0");
    logger.logExp("-> $$pr0 : ", rank1, DLL_INTERFACE_LOGGER);

    isValid = isValid && (rank1 >= 2 && rank1 <= 14);

    char crank1 = convertRankToChar((int)rank1);

    double suit1 = gws("$$ps0");    
    logger.logExp("-> $$ps0 : ", suit1, DLL_INTERFACE_LOGGER);

    char csuit1 = convertSuitToChar((int)suit1);
        
    logger.logExp("-> rank1 : ", crank1, DLL_INTERFACE_LOGGER);
    logger.logExp("-> suit1 : ", csuit1, DLL_INTERFACE_LOGGER);

    double rank2 = gws("$$pr1");
    isValid = isValid && (rank1 >= 2 && rank1 <= 14);

    char crank2 = convertRankToChar((int)rank2);
    double suit2 = gws("$$ps1");
    char csuit2 = convertSuitToChar((int)suit2);
    logger.logExp("-> rank2 : ", rank2, DLL_INTERFACE_LOGGER);
    logger.logExp("-> suit2 : ", suit2, DLL_INTERFACE_LOGGER);
    
    Card hero1(crank1, csuit1);
    Card hero2(crank2, csuit2);
    Hand holeCard(hero1, hero2);
    currentGameInfo->setHand(holeCard);

    char buffer[100];
    sprintf(buffer,"-> hole cards : %c%c%c%c", crank1, csuit1, crank2, csuit2);
    logger.logExp(buffer, DLL_INTERFACE_LOGGER);
    // common cards
    vector<Card> board;
    // flop
	if (bettingRound > 1)
	{
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
	}

    // turn
	if (bettingRound > 2)
	{
		double turnrank = gws("$$cr3");
		double turnsuit = gws("$$cs3");
		char trank = convertRankToChar((int)turnrank);
		char tsuit = convertSuitToChar((int)turnsuit);
		Card turn(trank, tsuit);
		board.push_back(turn);
	}

	    // river
	if (bettingRound > 3)
	{

		double riverrank = gws("$$cr4");    
		double riversuit = gws("$$cs4");
		char rrank = convertRankToChar((int)riverrank);
		char rsuit = convertSuitToChar((int)riversuit);
		Card river(rrank, rsuit);
		board.push_back(river);
	}
    currentGameInfo->setBoard(board);

    double amountToCall = gws("call");
    logger.logExp("-> amount to call : ", amountToCall, DLL_INTERFACE_LOGGER);
    currentGameInfo->setAmountToCall(amountToCall / bblind);

    double potCommon = gws("potcommon");
    logger.logExp("-> pot common : ", potCommon / bblind, DLL_INTERFACE_LOGGER);
    currentGameInfo->setPotcommon(potCommon / bblind);
    
    double totalPot = gws("pot");
    logger.logExp("-> total in pot : ", (totalPot) / bblind, DLL_INTERFACE_LOGGER);
    currentGameInfo->setTotalPot(totalPot / bblind);

    return currentGameInfo;
}

bool isBitSet(int toTest, int bitNumber)
{
    int bitposition = 1 << bitNumber;
    return (toTest & bitposition) == bitposition;
}

bool isEqual(double d1, double d2)
{
    double eps = 0.01;
    return ( d1 - eps < d2 ) && ( d2 - eps < d1);
}

double getBalanceByPos(int idx)
{    
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	GameStateManager& gameStateManager = GameStateManager::getGameStateManager();

    char buffer[20];
    sprintf(buffer, "balance%d", idx);

    double balance = gws(buffer);
	if (idx == 0)
	{
		if (balance > 0.01)
		{
			logger.logExp("Setting hero balance : ", balance, BOT_LOGIC);
			gameStateManager.setMyStackSize(balance);
		}
		else
		{
			balance = gameStateManager.getMyStackSize();
		}
	}

	if (balance < 0.01) balance = 100000;

    return balance;
}

void getCurrentBets(vector<double>& currentBets, double bblind)
{
	Logger& logger = Logger::getLogger(LOGGER_TYPE::DLL_INTERFACE_LOGGER);
	
    currentBets.clear();

    char buffer[100];
    for (int idx = 0; idx < 6; ++idx)
    {
        sprintf(buffer, "currentbet%d", idx);
		logger.logExp("-> symbol : ", buffer, LOGGER_TYPE::DLL_INTERFACE_LOGGER);
        double currentBet = gws(buffer);
		logger.logExp("-> currentbet : ", currentBet, LOGGER_TYPE::DLL_INTERFACE_LOGGER);
        currentBets.push_back(currentBet / bblind);
    }
}

void refreshPlayersName(holdem_state* pstate)
{

	if (pstate == NULL) return ;

    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);

    GameStateManager& gameStateManager = GameStateManager::getGameStateManager();
	PlayerRangeManager& playerRangeManager = PlayerRangeManager::getPlayerRangeManager();

    char buffer[100];
    for (int idx = 0; idx <= 5; ++idx)
    {        
        holdem_player& hp = pstate->m_player[idx];
        
        if (hp.m_name_known & 0x01)
        {
            sprintf(buffer, "holdem_player[%d] -> player name : ", idx);
            logger.logExp(buffer, hp.m_name, DLL_INTERFACE_LOGGER);

            gameStateManager.setPlayer(string(hp.m_name), idx);
        }
    }
}

void resetHand(holdem_state* pstate, Hand hand)
{
    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);
    logger.logExp("[New hand] : resetting game state!", DLL_INTERFACE_LOGGER);

    double dealerchair = gws("dealerchair");
    logger.logExp("-> dealerchair : ", dealerchair, DLL_INTERFACE_LOGGER);

    GameStateManager& gameStateManager = GameStateManager::getGameStateManager();
    gameStateManager.resetState((int)dealerchair, hand);
	scrape_cycle = 0;
    // setting up starting balances
    for (int idx = 0; idx <= 5; ++idx)
    {        
        gameStateManager.setInitialBalance(idx, 0);
    }
}

void correctStackSizes(CurrentPlayerInfo& player, double currentBet, int idx)
{
	GameStateManager& gamestateManager = GameStateManager::getGameStateManager();
	double bblind = gamestateManager.getCurrentGameInfo()->getBblind();

	double actualStack = player.getActualStacksize();
	double detectedActualStack = getBalanceByPos(idx) / bblind;
	if (abs(actualStack - detectedActualStack) < 0.01)
	{
		detectedActualStack -= currentBet; 
		detectedActualStack += player.getBetsize();
	}
	player.setBetsize(currentBet);
	player.setActualStacksize(detectedActualStack);
}

void detectMissedChecksAndUpdatePlayerRanges(CurrentGameInfo *old_cgi)
{
	if (old_cgi == NULL)
	{
		return ;
	}

	GameStateManager& gamestateManager = GameStateManager::getGameStateManager();
	Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);

    BotManager& botManager = BotManager::getBotManager();
    AbstractBotLogic* botLogic = botManager.getPluggableBot();

    PlayerRangeManager& playerRangeManager = PlayerRangeManager::getPlayerRangeManager();

    double maxRaise = gamestateManager.getMaxRaise();
	double bblind = gamestateManager.getCurrentGameInfo()->getBblind();

	vector<int> relativePositions;
    calculateRelativPositions(relativePositions, gamestateManager.getDealerPosition(), true);

	if (isEqual(maxRaise,0))
	{
		double playersplayingbits = gws("playersplayingbits");

		for (int idx = 1; idx < 6; ++idx)
		{
			if (isBitSet((int)playersplayingbits, idx) && (relativePositions[0] < relativePositions[idx]))
			{
				if (gamestateManager.isCurrentPlayerInfoSet(idx))
				{	
					CurrentPlayerInfo& currentPlayerInfo = gamestateManager.getCurrentPlayerInfo(idx);
					logger.logExp("DETECTED : Check " + currentPlayerInfo.getName(), DLL_INTERFACE_LOGGER);
					
					currentPlayerInfo.setLine(2);
					currentPlayerInfo.setBetsize(0);

					if (gamestateManager.getCurrentGameInfo()->getStreet() == 0) currentPlayerInfo.setStacksize(gamestateManager.getInitialBalanceByPos(idx) / bblind);

					old_cgi->addCurrentPlayerInfo(gamestateManager.getCurrentPlayerInfo(idx));
					
					PlayerRange& range = playerRangeManager.getPlayerRange(idx);
					PlayerRange updatedRange = botLogic->calculateRange(idx, *old_cgi, range);

					if (!updatedRange.getValid())
					{
						logger.logExp("UpdatedRange set to false1\n", BOT_LOGIC);
					}

					updatedRange.setId(idx);


					logger.logExp("Cache set to false CheckHevele", DLL_DECISION_LOGGER);
					gamestateManager.setCache(false);
					playerRangeManager.setPlayerRange(idx, updatedRange);
				}
				else
				{
					logger.logExp("ERROR : Existing active player with no info set.",DLL_INTERFACE_LOGGER);
				}
			}
		}
	}
}

void detectMissedCallsAndUpdatePlayerRanges(CurrentGameInfo *old_cgi)
{
	Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);

	if (old_cgi == NULL)
	{
		return ;
	}

    GameStateManager& gamestateManager = GameStateManager::getGameStateManager();

    BotManager& botManager = BotManager::getBotManager();
    AbstractBotLogic* botLogic = botManager.getPluggableBot();

    PlayerRangeManager& playerRangeManager = PlayerRangeManager::getPlayerRangeManager();

    double maxRaise = gamestateManager.getMaxRaise();
	double bblind = gamestateManager.getCurrentGameInfo()->getBblind();

	vector<int> relativePositions;
    calculateRelativPositions(relativePositions, gamestateManager.getDealerPosition());

    double playersplayingbits = gws("playersplayingbits");
    for (int idx = 0; idx < 6; ++idx)
    {        
        if (isBitSet((int)playersplayingbits, idx))
        {
			// If this player's info is not set, we should set it here
			if (!gamestateManager.isCurrentPlayerInfoSet(idx))
			{
				
				CurrentPlayerInfo currentPlayerInfo;
                        
				currentPlayerInfo.setStacksize(gamestateManager.getInitialBalanceByPos(idx) / bblind);
				currentPlayerInfo.setActualStacksize(getBalanceByPos(idx) / bblind);
				currentPlayerInfo.setLine((isEqual(maxRaise,1.0) && relativePositions[idx] == 2) ? 2 : 0);
				currentPlayerInfo.setBetsize((isEqual(maxRaise,1.0) && relativePositions[idx] == 2) ? 0 : maxRaise);
				currentPlayerInfo.setName(gamestateManager.getPlayerNameByPos(idx));
				currentPlayerInfo.setPoz(relativePositions[idx]);
				currentPlayerInfo.setId(idx);

				logger.logExp("DETECTED : Call, currentplayer info not set " + currentPlayerInfo.getName(), DLL_INTERFACE_LOGGER);
				
				if (idx > 0)
				{
					gamestateManager.setCurrentPlayerInfo(idx, currentPlayerInfo);

					old_cgi->addCurrentPlayerInfo(currentPlayerInfo);
					
					// let's update
					PlayerRange pr = playerRangeManager.getPlayerRange(idx);
					pr.setId(idx);
					PlayerRange updatedRange = botLogic->calculateRange(idx, *old_cgi, pr);
					updatedRange.setId(idx);

					if (!updatedRange.getValid())
					{
						logger.logExp("UpdatedRange set to false2\n", BOT_LOGIC);
					}

					gamestateManager.setCache(false);
					logger.logExp("Cache set to false Detected call", DLL_DECISION_LOGGER);
					playerRangeManager.setPlayerRange(idx, updatedRange);
				}
				else
				{
					gamestateManager.setCurrentPlayerInfo(0, currentPlayerInfo);
				}
			}
			else
			{
				CurrentPlayerInfo& currentPlayerInfo = gamestateManager.getCurrentPlayerInfo(idx);

				if (!isEqual(maxRaise, currentPlayerInfo.getBetsize()))
				{
					logger.logExp("DETECTED : Call, currentplayer info set " + currentPlayerInfo.getName(), DLL_INTERFACE_LOGGER);
					if (gamestateManager.getCurrentGameInfo()->getStreet() == 0) currentPlayerInfo.setStacksize(gamestateManager.getInitialBalanceByPos(idx) / bblind);
					currentPlayerInfo.setLine(0);
					correctStackSizes(currentPlayerInfo, maxRaise, idx);

					if (gamestateManager.getCurrentGameInfo()->getStreet() == 0)
					{
						if (abs(currentPlayerInfo.getBetsize()-1) < 0.01 && currentPlayerInfo.getPoz() != 2)
						{
							currentPlayerInfo.setVPIP(40);
							currentPlayerInfo.setPFR(10);
							currentPlayerInfo.setAF(1);
						}
					}

					if (idx > 0)
					{
						old_cgi->addCurrentPlayerInfo(gamestateManager.getCurrentPlayerInfo(idx));
					
						PlayerRange& range = playerRangeManager.getPlayerRange(idx);
						PlayerRange updatedRange = botLogic->calculateRange(idx, *old_cgi, range);
						updatedRange.setId(idx);

						if (!updatedRange.getValid())
						{
							logger.logExp("UpdatedRange set to false3\n", BOT_LOGIC);
						}	


						gamestateManager.setCache(false);
						logger.logExp("Cache set to false Detected call", DLL_DECISION_LOGGER);
						playerRangeManager.setPlayerRange(idx, updatedRange);
					}
					else
					{
						gamestateManager.setCurrentPlayerInfo(0, currentPlayerInfo);
					}
				}
			}
        }
    }
}

void detectChecksAndUpdateRanges(int relativTo)
{
    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER); 
    logger.logExp("[Processing in detectChecksAndUpdateRanges]", DLL_INTERFACE_LOGGER);

    GameStateManager& gamestateManager = GameStateManager::getGameStateManager();

    BotManager& botManager = BotManager::getBotManager();
    AbstractBotLogic* botLogic = botManager.getPluggableBot();

    PlayerRangeManager& playerRangeManager = PlayerRangeManager::getPlayerRangeManager();

    vector<int> relativePositions;
    calculateRelativPositions(relativePositions, gamestateManager.getDealerPosition(), true);

    vector<double> currentBets(6);
    getCurrentBets(currentBets, BBLIND);

	logger.logExp("DETECTING : Check ", DLL_INTERFACE_LOGGER);

    double playersplayingbits = gws("playersplayingbits");
    for (int idx = 1; idx < 6; ++idx)
    {        
        if (isBitSet((int)playersplayingbits, idx))
        {
			if (!gamestateManager.isCurrentPlayerInfoSet(idx)) 
            {
                logger.logExp("--> ERROR : CurrentPlayerInfo is not set!", DLL_INTERFACE_LOGGER);
                continue;
            }

            if (isEqual(currentBets[idx], 0.0) && (relativePositions[idx] < relativePositions[relativTo] ) )
            {
                CurrentPlayerInfo& currentPlayerInfo = gamestateManager.getCurrentPlayerInfo(idx);
			
				if (currentPlayerInfo.getLine() == 2) continue;
				if (gamestateManager.getCurrentGameInfo()->getStreet() == 0) currentPlayerInfo.setStacksize(gamestateManager.getInitialBalanceByPos(idx) / gamestateManager.getCurrentGameInfo()->getBblind());
				logger.logExp("DETECTED : Check Boldifele " + currentPlayerInfo.getName(), DLL_INTERFACE_LOGGER);
                currentPlayerInfo.setLine(2);
			    currentPlayerInfo.setBetsize(0);

                CurrentGameInfo* cgi = gamestateManager.getCurrentGameInfo();
                if (cgi != NULL)
                {
			        cgi->addCurrentPlayerInfo(currentPlayerInfo);
                }
                else
                {
                    logger.logExp("--> ERROR : CurrentGameInfo is null!", DLL_INTERFACE_LOGGER);
                }
					
				PlayerRange& range = playerRangeManager.getPlayerRange(idx);
                PlayerRange updatedRange = botLogic->calculateRange(idx, *cgi, range);
				updatedRange.setId(idx);

				if (!updatedRange.getValid())
				{
					logger.logExp("UpdatedRange set to false4\n", BOT_LOGIC);
				}

			    gamestateManager.setCache(false);
				logger.logExp("Cache set to false CheckBoldifele", DLL_DECISION_LOGGER);
                playerRangeManager.setPlayerRange(idx, updatedRange);
            }
        }
    }
}

void refreshStackSizes(vector<double>& currentBets)
{
	GameStateManager& gameStateManager = GameStateManager::getGameStateManager();

    for (int idx = 0; idx <= 5; ++idx)
    {   
		double stacksize = getBalanceByPos(idx) + currentBets[idx];
		double oldStacksize = gameStateManager.getInitialBalanceByPos(idx);

		if (oldStacksize < stacksize)
		{
			gameStateManager.setInitialBalance(idx, stacksize);
			if (gameStateManager.isCurrentPlayerInfoSet(idx))
			{
				CurrentPlayerInfo& player = gameStateManager.getCurrentPlayerInfo(idx);
				player.setStacksize(stacksize / gameStateManager.getCurrentGameInfo()->getBblind());
			}
		}
    }
}

double AnyVPIP(int index)
{
	Logger& logger = Logger::getLogger(DLL_DECISION_LOGGER);
	
	GameStateManager& gamestateManager = GameStateManager::getGameStateManager();

	Database* database = gamestateManager.getDatabase();

	vector<int> relativePositions;
    calculateRelativPositions(relativePositions, gamestateManager.getDealerPosition());

	int idx = 0;
	for (; idx < 6; ++idx)
	{
		if (relativePositions[idx] == index) break;
	}

	string name = gamestateManager.getPlayerNameByPos(idx);
	
	logger.logExp("DETECTED Query " + name,DLL_DECISION_LOGGER);
	if (name == "") return 20;

	double VPIP = 0;

	if (database->isUser(name))
	{
		VPIP = database->getVPIP(name);
		logger.logExp("VPIP got: ", VPIP,DLL_DECISION_LOGGER);
	}
	else
	{
		VPIP = 20;
	}

	return VPIP;
}

double process_state(holdem_state* pstate);

double process_query(const char* pquery)
{

	if (!strcmp(pquery,"dll$BUVPIP"))
	{
		return AnyVPIP(0);
	}

	if (!strcmp(pquery,"dll$BBVPIP"))
	{
		return AnyVPIP(2);
	}

	Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER); 
	logger.logExp(string("[Processing query] : ").append(pquery).c_str(), DLL_DECISION_LOGGER);

	if (strcmp(pquery,"dll$swag") && strcmp(pquery,"dll$srai") && strcmp(pquery,"dll$call") && strcmp(pquery,"dll$prefold"))
		return 0;
    
	int total = 0;
	while (process_state(NULL)==-1)
	{
		if (total++ > 10) break;
	}
	//LET THE OLD BOT DECIDE
	if (process_state(NULL) == -1) return -1;


	if(pquery == NULL)
    {
		return 0;
    }

    BotManager& botManager = BotManager::getBotManager();
    AbstractBotLogic* botLogic = botManager.getPluggableBot();

    GameStateManager& gamestateManager = GameStateManager::getGameStateManager();
    PlayerRangeManager& playerRangeManager = PlayerRangeManager::getPlayerRangeManager();

	if (!gamestateManager.getHand().valid())
	{
		logger.logExp("Hand validation failed. Returning.", DLL_DECISION_LOGGER);
		return 0;
	}

	logger.logExp("[-> hand number] : ", gamestateManager.getHandNumber(), BOT_LOGIC);

	double playersplayingbits = gws("playersplayingbits");

    detectChecksAndUpdateRanges(0); // here we can just detect whom are  already made a check
    //
	vector<PlayerRange> allRanges = playerRangeManager.getPlayerRanges();
    vector<PlayerRange> ranges;

	for (int idx = 1; idx <=5; ++idx)
	{
		if (isBitSet((int)playersplayingbits, idx) && gamestateManager.isCurrentPlayerInfoSet(idx))
		{
			if (!allRanges[idx].getValid())
			{
				logger.logExp("range is not valid", BOT_LOGIC);
			}
			ranges.push_back(allRanges[idx]);
		}
	}

	Action action;
	if (gamestateManager.isCacheAvalaible())
	{
		action = gamestateManager.getAction();
		logger.logExp("Action is cached: " + action.toString(), DLL_DECISION_LOGGER);
	}
	else
	{
		vector<int> postflopRelatives;
		calculateRelativPositions(postflopRelatives, gamestateManager.getDealerPosition(), true);

		// wait for hero's balance when the action before was raise and we are oop against 1 opponent
		if (ranges.size() == 1 && gamestateManager.getAction().getType() == 'r')
		{
			logger.logExp("Waiting for hero's balance", DLL_DECISION_LOGGER);
			Sleep(3000);
			process_state(NULL);
		}

		// TODO : implement advanced bluffing here
		if (gamestateManager.isBluff())
		{

		}

		CurrentGameInfo* cgi = gamestateManager.getCurrentGameInfo();
		vector<double> currentBets(6);
		getCurrentBets(currentBets, cgi->getBblind());
		
		double potcommon = cgi->getPotcommon();

		//HERO SHOULD BE THERE?
		for (int idx = 1; idx <=5; ++idx)
		{
			if (isBitSet((int)playersplayingbits, idx) && gamestateManager.isCurrentPlayerInfoSet(idx))
			{
				CurrentPlayerInfo& player = gamestateManager.getCurrentPlayerInfo(idx);
				
				player.setStacksize(gamestateManager.getInitialBalanceByPos(idx) / cgi->getBblind());
				if (cgi->getStreet() == 0 && (player.getActualStacksize() + player.getBetsize() > player.getStacksize() + 0.01)) 
					player.setActualStacksize(player.getStacksize() - player.getBetsize());
				cgi->addCurrentPlayerInfo(player);
			}
		}

		for (int idx = 1; idx <=5; ++idx)
		{
			if (!isBitSet((int)playersplayingbits, idx))
			{
				if (currentBets[idx] > 0)
					potcommon += currentBets[idx] / cgi->getBblind(); 
			}
		}

		vector<int> relativPositions;
		calculateRelativPositions(relativPositions, gamestateManager.getDealerPosition());

		//OTHER PLAYERS TOO MAYBE?
		gamestateManager.getCurrentPlayerInfo(0).setPoz(relativPositions[0]);
		gamestateManager.getCurrentPlayerInfo(0).setActualStacksize(getBalanceByPos(0) / cgi->getBblind());
		cgi->setHero(gamestateManager.getCurrentPlayerInfo(0));

		action = botLogic->makeDecision(*cgi, ranges);

		if (action.isBluff())
		{
			gamestateManager.setBluff(true);
		}
	} 

	//WriteToDebugWindow();

	logger.logExp("Got action: " + action.toString(), DLL_DECISION_LOGGER);

    if(strcmp(pquery,"dll$swag") == 0)
    {
		
		double result = 0.0;
		if (action.getType() == 'n')
		{
			result = -1;
		}
		else
		{
			if (action.getType() == 'h')
			{
				logger.logExp("Action is hack dll$swag\n", DLL_DECISION_LOGGER);
				result = -1;
			}
			else
			{
				result = action.getSize();
			}
		}

		gamestateManager.setAction(action);
		gamestateManager.setCache(true);
		//logger.logExp("Action is cached\n");
		//if (action.getType() == 'r') Sleep(1000);

		return result;
    }

    if(strcmp(pquery,"dll$srai") == 0)
    {
		if (action.getType() == 'n')
		{
			return -1;
		}

		gamestateManager.setAction(action);
		gamestateManager.setCache(true);
        return action.getType() == 'r';
    }    

    if(strcmp(pquery,"dll$call")==0)
    {
		if (action.getType() == 'n')
		{
			return -1;
		}

		if (action.getType() == 'h')
		{
			logger.logExp("Action is hack dll$call\n", DLL_DECISION_LOGGER);
			return 1;
		}

		gamestateManager.setAction(action);
		gamestateManager.setCache(true);
		return action.getType() == 'c';
    }

    if(strcmp(pquery,"dll$prefold")==0)
    {
		if (action.getType() == 'n')
		{
			return -1;
		}

		gamestateManager.setAction(action);
		gamestateManager.setCache(true);
		return action.getType() == 'f';
    }

	return 0;

}


bool validState(CurrentGameInfo* cgi, vector<double>& currentBets)
{
	Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);

	GameStateManager& gamestateManager = GameStateManager::getGameStateManager();
	
	if (cgi->getPotcommon() < 0) return false;

	if (isEqual(cgi->getStreet(), gamestateManager.getBettingRound()) && !gamestateManager.IsHandReset(cgi->getHandNumber()))
	{
		for (int i = 0; i < currentBets.size(); ++i)
		{
			if (currentBets[i] < 0) return false;

			if (gamestateManager.isCurrentPlayerInfoSet(i))
				if (currentBets[i] < gamestateManager.getCurrentBetByPos(i)-0.01) return false;
		}
	}
	double totalpot = cgi->getPotcommon();
	logger.logExp("Common pot :", totalpot, DLL_INTERFACE_LOGGER);
	for (int i = 0; i < currentBets.size(); ++i)
	{
		logger.logExp("Adding current bet :", currentBets[i], DLL_INTERFACE_LOGGER);
		totalpot += currentBets[i];
	}
	logger.logExp("Total pot calculated:", totalpot, DLL_INTERFACE_LOGGER);
	logger.logExp("Total pot :", cgi->getTotalPot(), DLL_INTERFACE_LOGGER);
	if (!isEqual(cgi->getTotalPot(), totalpot)) return false;

	return true;
}



double process_state(holdem_state* pstate)
{
    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);
	logger.logExp("[Processing state] : ", DLL_INTERFACE_LOGGER);

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
        logger.logExp("[SKIPPING SCRAPPING CYCLE] : hero hole cards are not valid!", DLL_INTERFACE_LOGGER);
        return -1;
    }

	vector<double> currentBets(6);
    getCurrentBets(currentBets, cgi->getBblind());

	if (!validState(cgi, currentBets))
	{
		delete cgi;
        logger.logExp("[SKIPPING SCRAPPING CYCLE] : Not valid commmon pot!", DLL_INTERFACE_LOGGER);
		return -1;
	}

	CurrentGameInfo* old_cgi = gamestateManager.getCurrentGameInfo();
	if (old_cgi != NULL)
		cgi->setFlopPotSize(old_cgi->getFlopPotSize());
    gamestateManager.setCurrentGameInfo(cgi);

    // testing new hand   
	if (gamestateManager.IsHandReset(cgi->getHandNumber()))
    {
		gamestateManager.setCache(false);
		gamestateManager.setBluff(false);
		//update database
		logger.logExp("HandReset Cache = false", DLL_DECISION_LOGGER);
		gamestateManager.setHandNumber(cgi->getHandNumber());
        resetHand(pstate, cgi->getHand());

		playerRangeManager.resetRanges(gamestateManager);
		refreshPlayersName(pstate);
    }

	if (cgi->getStreet() == 0)
	{
		for (int idx = 0; idx < 6; ++idx)
		{
			if (currentBets[idx] > 0)
			{
				gamestateManager.setInitialBalance(idx, getBalanceByPos(idx) + currentBets[idx]);
			}
			else
			{
				gamestateManager.setInitialBalance(idx, getBalanceByPos(idx));
			}

			//logger.logExp("Initial balance set to: ", gamestateManager.getInitialBalanceByPos(idx), BOT_LOGIC);
		}
	}

	refreshPlayersName(pstate);

    // testing if we advanced to the next betting round
    if (cgi->getStreet() > gamestateManager.getBettingRound())
    {
		logger.logExp("betround Cache = false", DLL_DECISION_LOGGER);

		//Opponent Number == 1!!
		if (gamestateManager.getAction().getType() == 'r')
		{
			Sleep(3000);
		}
		

		gamestateManager.setCache(false);
        detectMissedCallsAndUpdatePlayerRanges(old_cgi);
		detectMissedChecksAndUpdatePlayerRanges(old_cgi);
		if (cgi->getStreet() == 1)
			cgi->setFlopPotSize(cgi->getPotcommon());
        gamestateManager.resetBettingRound();
    }

	/*
	if (cgi->getStreet() == 0)
	{
		refreshStackSizes(currentBets);
	}
	*/

	if (old_cgi)
	{
		delete old_cgi;
	}

	//ITS DUMMY HERE
    vector<int> relativePositions;
    calculateRelativPositions(relativePositions, gamestateManager.getDealerPosition());

    double playersplayingbits = gws("playersplayingbits");
    logger.logExp("-> playersplayingbits : ", playersplayingbits, DLL_INTERFACE_LOGGER);
    for (int idx = 0; idx < 6; ++idx) // hero always sits at 0!
    {
		// skip this frame if someone's name is unknown - this way we can avoid a lot of crashes
		if (gamestateManager.getPlayerNameByPos(idx) == "")
			return 0;

        char buffer[100];            
        if (isBitSet((int)playersplayingbits, idx))
        {
            sprintf_s(buffer, "--> Player %d is playing!",idx);
            logger.logExp(buffer, DLL_INTERFACE_LOGGER);

            double currentBet = currentBets[idx];

			if (!isEqual(currentBet, gamestateManager.getCurrentBetByPos(idx)))
            {
                if (gamestateManager.isCurrentPlayerInfoSet(idx))
                {
                    CurrentPlayerInfo& currentPlayerInfo = gamestateManager.getCurrentPlayerInfo(idx);

					double bblind = cgi->getBblind();

					if (cgi->getStreet() == 0) currentPlayerInfo.setStacksize(gamestateManager.getInitialBalanceByPos(idx) / bblind);

                    double maxRaise = gamestateManager.getMaxRaise();
                    if (isEqual(currentBet, maxRaise))
                    {
                        currentPlayerInfo.setLine(0);
						correctStackSizes(currentPlayerInfo, currentBet, idx);
						if (gamestateManager.getCurrentGameInfo()->getStreet() == 0)
						{
							if (abs(currentPlayerInfo.getBetsize()-1) < 0.01)
							{
								currentPlayerInfo.setVPIP(40);
								currentPlayerInfo.setPFR(10);
								currentPlayerInfo.setAF(1);
							}
						}
                    }
                    else if (currentBet > maxRaise)
                    {
                        currentPlayerInfo.setLine(1);
                        gamestateManager.setMaxRaise(currentBet);
						correctStackSizes(currentPlayerInfo, currentBet, idx);
                    }
                    else
                    {
                        // this is impossible - something went wrong :(
                        logger.logExp("[ERROR at setting last line !]", DLL_INTERFACE_LOGGER);
						return -1;
                    }

                    // this player did something so we update his/her range
                    string playerName = currentPlayerInfo.getName();
                    PlayerRange playerRange = playerRangeManager.getPlayerRange(idx);

					// needed by botlogic
					cgi->addCurrentPlayerInfo(currentPlayerInfo);
					
                    //playerRange.set
                    
					if (idx > 0)
					{
                        if (cgi->getStreet() > 0)
                        {
                            detectChecksAndUpdateRanges(idx);
                        }

						PlayerRange& range = playerRangeManager.getPlayerRange(idx);
						PlayerRange updatedRange = botLogic->calculateRange(idx, *cgi, range);
						updatedRange.setId(idx);


						logger.logExp("1 Cache = false", DLL_DECISION_LOGGER);
						gamestateManager.setCache(false);
						playerRangeManager.setPlayerRange(idx, updatedRange);

						
						if (!updatedRange.getValid())
						{
							logger.logExp("UpdatedRange set to false5\n", BOT_LOGIC);
						}

					}
					else
					{
						cgi->setHero(currentPlayerInfo);
					}
                }
                else
                {
					double bblind = cgi->getBblind();

                    CurrentPlayerInfo currentPlayerInfo;
                        
                    currentPlayerInfo.setStacksize(gamestateManager.getInitialBalanceByPos(idx) / bblind);
                    currentPlayerInfo.setName(gamestateManager.getPlayerNameByPos(idx));
                    currentPlayerInfo.setPoz(relativePositions[idx]);
					currentPlayerInfo.setId(idx);

                    double maxRaise = gamestateManager.getMaxRaise();
                    if (isEqual(currentBet, maxRaise))
                    {
                        currentPlayerInfo.setLine(0);
						correctStackSizes(currentPlayerInfo, currentBet, idx);
						if (gamestateManager.getCurrentGameInfo()->getStreet() == 0 && currentPlayerInfo.getPoz() != 2)
						{
							if (abs(currentPlayerInfo.getBetsize()-1) < 0.01)
							{
								currentPlayerInfo.setVPIP(40);
								currentPlayerInfo.setPFR(10);
								currentPlayerInfo.setAF(1);
							}
						}
                    }
                    else if (currentBet > maxRaise)
                    {
                        currentPlayerInfo.setLine(1);
                        gamestateManager.setMaxRaise(currentBet);
						correctStackSizes(currentPlayerInfo, currentBet, idx);
                    }
                    else
                    {
                        // this is impossible - something went wrong :(
                        logger.logExp("[ERROR at setting last line !]", DLL_INTERFACE_LOGGER);
						//TODO THIS NEEDS TO BE DONE
						return -1;
                    }

                    gamestateManager.setCurrentPlayerInfo(idx, currentPlayerInfo);

					cgi->addCurrentPlayerInfo(currentPlayerInfo);
					
                    // let's update

					if (idx > 0)
					{
                        if (cgi->getStreet() > 0)
                        {
                            detectChecksAndUpdateRanges(idx);
                        }

						PlayerRange pr = playerRangeManager.getPlayerRange(idx);
						pr.setId(idx);
						if (!pr.getValid())
						{
							logger.logExp("Given range set to false6\n", BOT_LOGIC);
						}

						PlayerRange updatedRange = botLogic->calculateRange(idx, *cgi, pr);
						updatedRange.setId(idx);
						
						if (!updatedRange.getValid())
						{
							logger.logExp("UpdatedRange set to false6\n", BOT_LOGIC);
						}

						logger.logExp("2 Cache = false", DLL_DECISION_LOGGER);
						gamestateManager.setCache(false);
						
						playerRangeManager.setPlayerRange(idx, updatedRange);
					}
					else
					{
						cgi->setHero(currentPlayerInfo);
					}
                }
            }
        }
        else
        {
            sprintf_s(buffer, "--> Player %d has already folded!",idx);
            logger.logExp(buffer, DLL_INTERFACE_LOGGER);
        }
    }
	
	 ++scrape_cycle;
	//WriteToDebugWindow();

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



//WndProc for the new window
LRESULT CALLBACK DLLWindowProc (HWND, UINT, WPARAM, LPARAM);

//Register our windows Class
BOOL RegisterDLLWindowClass(wchar_t szClassName[])
{
    WNDCLASSEXW wc;
    wc.hInstance =  inj_hModule;
	wc.lpszClassName = (LPCWSTR)L"InjectedDLLWindowClass";
    wc.lpszClassName = (LPCWSTR)szClassName;
    wc.lpfnWndProc = DLLWindowProc;
    wc.style = CS_DBLCLKS;
    wc.cbSize = sizeof (WNDCLASSEX);
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    if (!RegisterClassExW (&wc))
		return 0;
}
//Creating our windows Menu
HMENU CreateDLLWindowMenu()
{
	HMENU hMenu;
	hMenu = CreateMenu();
	HMENU hMenuPopup;
    if(hMenu==NULL)
        return FALSE;
    hMenuPopup = CreatePopupMenu();
	AppendMenu (hMenuPopup, MF_STRING, MYMENU_EXIT, TEXT("Exit"));
    AppendMenu (hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, TEXT("File")); 

	hMenuPopup = CreatePopupMenu();
    AppendMenu (hMenuPopup, MF_STRING,MYMENU_MESSAGEBOX, TEXT("MessageBox")); 
    AppendMenu (hMenu, MF_POPUP, (UINT_PTR) hMenuPopup, TEXT("Test")); 
	return hMenu;
}

//The new thread
DWORD WINAPI ThreadProc( LPVOID lpParam )
{
    MSG messages;
	wchar_t *pString = reinterpret_cast<wchar_t * > (lpParam);
	HMENU hMenu = CreateDLLWindowMenu();
	RegisterDLLWindowClass(L"InjectedDLLWindowClass");
	prnt_hWnd = FindWindowW(L"Window Injected Into ClassName", L"Window Injected Into Caption");
	hwnd = CreateWindowExW (0, L"InjectedDLLWindowClass", pString, WS_EX_PALETTEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, prnt_hWnd, hMenu,inj_hModule, NULL );
	ShowWindow (hwnd, SW_SHOWNORMAL);
    while (GetMessageW (&messages, NULL, 0, 0))
    {
		TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return 1;
}
//Our new windows proc
LRESULT CALLBACK DLLWindowProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_COMMAND:
               switch(wParam)
               {
                    case MYMENU_EXIT:
						SendMessage(hwnd, WM_CLOSE, 0, 0);
                        break;
                    case MYMENU_MESSAGEBOX:
						MessageBoxW(hwnd, L"Test", L"MessageBox",MB_OK);
                        break;
               }
               break;
		case WM_PAINT: 
			{
				PAINTSTRUCT ps;
				HDC hDC = BeginPaint (hwnd, &ps);

				HBRUSH background = CreateSolidBrush(RGB(0,155,0));
				FillRect(hDC, &ps.rcPaint, background);

				int iY = 5;
				for (int i = 0; i < myOutput.size(); ++i, iY += 20)
				{
					TextOut (hDC, 5, iY, myOutput[i].c_str(), myOutput[i].size());
				}
				EndPaint (hwnd, &ps);

				myOutput.clear();
			}
			break;
		case WM_DESTROY:
			PostQuitMessage (0);
			break;
		default:
			return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}



///////////////////////////////// 
//DLLMAIN 
///////////////////////////////// 
BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){

	switch(ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH:
			//inj_hModule = hModule;
			//CreateThread(0, NULL, ThreadProc, (LPVOID)L"Window Title", NULL, NULL);
			break; 
		case DLL_THREAD_ATTACH:
			//inj_hModule = hModule;
			//CreateThread(0, NULL, ThreadProc, (LPVOID)L"Window Title", NULL, NULL);
			break; 
		case DLL_THREAD_DETACH:
			break; 
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;

}