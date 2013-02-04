#include "bayesDecision.h"	
#include "eqcalculator.h"
#include "logger.h"
#include "evaluator.h"
#include <sstream>
#include <time.h>

bool inPosition(int poz1, int poz2)
{
	if (poz1 == 1) poz1 = -5;
	if (poz1 == 2) poz1 = -4;
	if (poz2 == 1) poz2 = -5;
	if (poz2 == 2) poz2 = -4;

	return poz1 > poz2;
}
bool heroInPosition(CurrentGameInfo& gameInfo)
{
	int myPoz = gameInfo.getHero().getPoz();
	bool inPoz = true;
	for (int i = 0; i < gameInfo.getOpponentsInfo().size(); ++i)
	{
		int poz = gameInfo.getOpponentsInfo()[i].getPoz();
		if (!inPosition(myPoz, poz))
		{
			inPoz = false;
		}
	}

	return inPoz;
}
double maxOpponentStack(CurrentGameInfo& game)
{
	double maxbet = game.getBiggestBet();
	double maxStackSize = 0;
	for (int i = 0; i < game.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo player = game.getOpponentsInfo()[i];
		double currentbet = maxbet - player.getBetsize();
		double actualStack = player.getActualStacksize() - currentbet;

		if (maxStackSize < actualStack) maxStackSize = actualStack;  
	}

	return maxStackSize;
}
double maxOpponentTotalStack(CurrentGameInfo& game)
{
	double maxbet = game.getBiggestBet();
	double maxStackSize = 0;
	for (int i = 0; i < game.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo player = game.getOpponentsInfo()[i];
		double actualStack = player.getActualStacksize() + player.getBetsize();
		if (maxStackSize < actualStack) maxStackSize = actualStack;  
	}

	return maxStackSize;
}
double maxOpponentOriginalStack(CurrentGameInfo& game)
{
	double maxbet = game.getBiggestBet();
	double maxStackSize = 0;
	for (int i = 0; i < game.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo player = game.getOpponentsInfo()[i];
		double actualStack = player.getStacksize();
		if (maxStackSize < actualStack) maxStackSize = actualStack;  
	}

	return maxStackSize;
}
bool isRegular(CurrentGameInfo& game)
{
	for (int i = 0; i < game.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo player = game.getOpponentsInfo()[i];
		if (player.getVPIP() < 22 && abs(player.getVPIP() - player.getPFR()) < 7)
		{
			return true;
		}
	}

	return false;
}
bool isFish(CurrentGameInfo& game)
{
	for (int i = 0; i < game.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo player = game.getOpponentsInfo()[i];
		if (player.getVPIP() > 25)
		{
			return true;
		}
	}

	return false;
}
double getMaxRaise(CurrentGameInfo& gameInfo)
{
	double betsize = 0;
	for (int i = 0; i < gameInfo.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo& player = gameInfo.getOpponentsInfo()[i];
		if (player.getBetsize() > betsize)
			betsize = player.getBetsize();
	}

	return betsize;
}
double modifyValue(double x, double value)
{
	x += value;
	if (x < 0) x = 0;
	if (x > 1) x = 1;

	return x;
}
double BayesDecision::nrOfPlayersWithMaxBetSize(CurrentGameInfo& gameInfo)
{
	double result = 0;

	vector<CurrentPlayerInfo>& players = gameInfo.getOpponentsInfo();

	for (int i = 0; i < players.size(); ++i)
	{
		if (abs(players[i].getBetsize() - gameInfo.getBiggestBet()) < 0.01)
			result ++;
	}

	return result;
}

vector<double> BayesDecision::getRaiseSizes(CurrentGameInfo& gameInfo)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	vector<double> result;

	int str = Evaluator::cardStrength(gameInfo.getHand().getCard1(), gameInfo.getHand().getCard2(), gameInfo.getBoard());
	int maxOpponentStack = maxOpponentOriginalStack(gameInfo);

	logger.logExp("\t\t========CALCULATING RAISE SIZES=========================", BOT_LOGIC);

	logger.logExp("\t\tHand Str: ", str, BOT_LOGIC);
	logger.logExp("\t\tMaxopponent original stack: ", maxOpponentStack, BOT_LOGIC);

	if (gameInfo.getStreet() == 0)
	{
		if (gameInfo.getBiggestBet() >= 10)
		{
			if (heroInPosition(gameInfo))
			{
				result.push_back( (gameInfo.getBiggestBet() * (2.2 + nrOfPlayersWithMaxBetSize(gameInfo) - 1)) * gameInfo.getBblind() );
			}
			else
			{
				result.push_back( (gameInfo.getBiggestBet() * (2.7 + nrOfPlayersWithMaxBetSize(gameInfo) - 1)) * gameInfo.getBblind() );
			}
		}
		else
		{
			if (heroInPosition(gameInfo))
				result.push_back( (gameInfo.getBiggestBet() * (2.5 + nrOfPlayersWithMaxBetSize(gameInfo) - 1)) * gameInfo.getBblind() );
			result.push_back( (gameInfo.getBiggestBet() * (3 + nrOfPlayersWithMaxBetSize(gameInfo) - 1)) * gameInfo.getBblind() );
			result.push_back( (gameInfo.getBiggestBet() * (3.5 + nrOfPlayersWithMaxBetSize(gameInfo) - 1)) * gameInfo.getBblind() );
		}
	}
	else
	{
		if ( gameInfo.getBiggestBet() < gameInfo.getPotcommon() / 5 )
		{
			int boardType = Evaluator::boardType(gameInfo.getBoard());
			double potcommon = gameInfo.getPotcommon();

			if ((boardType == 0 && potcommon >= 17 && gameInfo.getStreet() == 1) || (boardType == 0 && potcommon >= 32 && gameInfo.getStreet() == 2))
			{
				result.push_back( ( gameInfo.getPotcommon() * 1/2 ) * gameInfo.getBblind() );
			}
			else if ((potcommon >= 17 && gameInfo.getStreet() == 1) || (potcommon >= 32 && gameInfo.getStreet() == 2))
			{
				result.push_back( ( gameInfo.getPotcommon() * 1/2 ) * gameInfo.getBblind() );
				result.push_back( ( gameInfo.getPotcommon() * 2/3 ) * gameInfo.getBblind() );
			}
			else
			{
				
				if (gameInfo.getStreet() == 3)
				{
					result.push_back( ( gameInfo.getPotcommon() * 1/2 ) * gameInfo.getBblind() );
				}
				else if (heroInPosition(gameInfo))
				{
					result.push_back( ( gameInfo.getPotcommon() * 1/2 ) * gameInfo.getBblind() );
				}
			
				result.push_back( ( gameInfo.getPotcommon() * 2/3 ) * gameInfo.getBblind() );
				result.push_back( ( gameInfo.getPotcommon() * 3/4 ) * gameInfo.getBblind() );
				if (boardType != 0) result.push_back( ( gameInfo.getPotcommon() ) * gameInfo.getBblind() );
			}
		}
		else
		{

				double correction = 0;
				if ( gameInfo.getBiggestBet() < gameInfo.getPotcommon() / 3 )
					correction = gameInfo.getBiggestBet();

				if (heroInPosition(gameInfo))
					result.push_back( (gameInfo.getBiggestBet() * (2.5 + nrOfPlayersWithMaxBetSize(gameInfo) - 1) + correction ) * gameInfo.getBblind() );

				result.push_back( (gameInfo.getBiggestBet() * (3 + nrOfPlayersWithMaxBetSize(gameInfo) - 1) + correction ) * gameInfo.getBblind() );
				result.push_back( (gameInfo.getBiggestBet() * (3.5 + nrOfPlayersWithMaxBetSize(gameInfo) - 1) + correction ) * gameInfo.getBblind() );		
		}
		
		if (gameInfo.getStreet() == 3 && isFish(gameInfo))
				result.push_back( gameInfo.getHero().getActualStacksize() * gameInfo.getBblind() );
	}

	return result;
}
PlayerRange BayesDecision::getCallRaiseRange(double betsize, PlayerRange& range, CurrentGameInfo& game, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	PlayerRange totalRange;

	CurrentPlayerInfo player = game.getPlayerbyId(range.getId());
	double actualStackSize = player.getActualStacksize() + player.getBetsize();
	actualStackSize *= game.getBblind();
	if (betsize > actualStackSize) betsize = actualStackSize;

	if (game.getStreet() == 0)
	{
		totalRange = preflop.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getStacksize() * game.getBblind(), player.getPoz(), 0, betsize, game.getBblind(), 0, patternsNeeded);
	}

	if (game.getStreet() == 1)
	{
		totalRange = flop.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 0, betsize, game.getBblind(),
			game.getPotcommon() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
	}

	if (game.getStreet() == 2)
	{
		totalRange = turn.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 0, betsize, game.getBblind(),
			game.getPotcommon() * game.getBblind(),  game.getFlopPotSize() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
	}

	if (game.getStreet() == 3)
	{
		totalRange = river.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 0, betsize, game.getBblind(),
			game.getPotcommon() * game.getBblind(),  game.getFlopPotSize() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
	}

	ostringstream s;
	s << "Betsize: " << betsize << " ";
	s << "VPIP: " << player.getVPIP() << " ";
	s << "PFR: " << player.getPFR() << " ";
	s << "AF: " << player.getAF() << " ";
	s << "StackSize: " << player.getStacksize() << " ";
	s << "bblind: " << game.getBblind() << " ";
	s << "potcommon: " << game.getPotcommon() << endl;

	s << "board: ";
	vector<Card> board = game.getBoard();
	for (int i = 0; i < board.size(); ++i)
	{
		s << board[i].toString();
	}

	s << endl << "hand: ";
	s << game.getHand().toString() << endl;

	logger.logExp(s.str(), BOT_LOGIC);

	totalRange.setId(range.getId());
	if (totalRange.range.size() == 0)
	{
		logger.logExp("Call raise range is 0", LOGGER_TYPE::BOT_LOGIC);
		return totalRange;
	}

	if (game.getStreet() != 0)
		totalRange = RangeUtils::mergeRange(range, totalRange, game.getBoard(), game.getHand());

	totalRange.setId(range.getId());
	if (totalRange.range.size() == 0)
	{
		logger.logExp("Merged range is 0", LOGGER_TYPE::BOT_LOGIC);
		return totalRange;
	}

	//s << "-----------------------------------------MERGED RANGE----------------------------------------" << endl << totalRange.toString() << endl << endl;

	totalRange.setId(range.getId());
	return totalRange;
}

//===================================MANIPULATE FUNCTIONS===============================================
double manipulateEQRaise(double EQ, double betsize, CurrentGameInfo& game)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);

	betsize /= game.getBblind();

	int maxStackSize = maxOpponentOriginalStack(game);
	double currentbet = game.getBiggestBet();

	if (game.getStreet() == 0)
	{
		//if (betsize < 24) EQ = modifyValue(EQ, 0.05);
		if (betsize < 24 && heroInPosition(game)) EQ = modifyValue(EQ, 0.05);
		//22-99  preflop raise -20%
		if (game.getHand().isPocket() && !game.getHand().getCard1().isBroadway() && maxStackSize > 30)
		{
			logger.logExp("\t\t\tModifying Raise: pocket EQ -20%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.2);
		}

		//FESZES ELLEN 4bet/fold mindent
		if (isRegular(game) && betsize > 30 && maxStackSize > 105 && !(game.getHand().isPocket() && game.getHand().getCard1().getRank() == 'A'))
		{
			logger.logExp("\t\t\tModifying Raise: if NOT AA EQ -30%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.30);
		}

		if (!heroInPosition(game) && betsize > 2 && !game.getHand().isOOP3Bet())
		{
			//OOP FOLD
			EQ = modifyValue(EQ, -0.20);
			logger.logExp("\t\t\tModifying Raise: 3BET OOP WEAK HAND EQ -20%", BOT_LOGIC);
		}

		if (betsize > 2 && (!game.getHand().isAxs() && !game.getHand().isStrongBroadway() && !game.getHand().isSuitedBroadway() && !game.getHand().isBigSC()))
		{
			//OOP FOLD
			EQ = modifyValue(EQ, -0.20);
			logger.logExp("\t\t\tModifying Raise: 3BET WEAK HAND EQ -20%", BOT_LOGIC);
		}

		if (betsize > 15 && !game.getHand().is100BBStackOff())
		{
			//OOP FOLD
			EQ = modifyValue(EQ, -0.20);
			logger.logExp("\t\t\tModifying Raise: 4BET WEAK HAND EQ -20%", BOT_LOGIC);
		}

		//IMPLEMENT PREFLOP 4BET
		if (betsize > 30 && maxStackSize > 105 && !game.getHand().is100BBStackOff())
		{
			EQ = modifyValue(EQ, -0.30);
			logger.logExp("\t\t\tModifying Raise: STACKOFF 100BB WEAK HAND EQ -20%", BOT_LOGIC);
		}

	}
	else if (game.getStreet() == 1)
	{
		if (!heroInPosition(game) && currentbet < 2)
		{
			logger.logExp("\t\t\tModifying Raise: OOP EQ -5%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.05);
		}

		if (currentbet >= 2 && maxStackSize > 100)
		{
			logger.logExp("\t\t\tModifying Raise: RAISE EQ -5%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.05);
		}
	}
	else if (game.getStreet() == 2)
	{
		if (!heroInPosition(game) && currentbet < 2)
		{
			logger.logExp("\t\t\tModifying Raise: OOP EQ -7%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.05);
		}

		if (currentbet >= 2 && maxStackSize > 100)
		{
			logger.logExp("\t\t\tModifying Raise: RAISE EQ -5%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.05);
		}
	}
	else if (game.getStreet() == 3)
	{
		if (currentbet >= 2 && maxStackSize > 100)
		{
			logger.logExp("\t\t\tModifying Raise: RAISE EQ -5%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.05);
		}
	}

	return EQ;
}
double manipulateEQRaiseAllIn(double EQ, double betsize, CurrentGameInfo& game)
{
	Logger& logger = logger.getLogger(BOT_LOGIC);

	betsize /= game.getBblind();

	double currentbet = game.getBiggestBet();
	double maxStackSize = maxOpponentTotalStack(game);


	double stacksize = game.getHero().getActualStacksize() + game.getHero().getBetsize();
	double maxOpponentS = maxOpponentTotalStack(game);
	double effectiv = (stacksize < maxOpponentS ? stacksize : maxOpponentS);

	//HA REGULAR LEHUZZUK RAISE-nel
	if (isRegular(game) && currentbet > 2 && maxStackSize > 120) 
	{
		logger.logExp("\t\t\tModifying AI: REG in play EQ -10%", BOT_LOGIC);
		EQ = modifyValue(EQ, -0.1);
	}

	if (game.getStreet() == 0)
	{
		//FESZES ELLEN 4bet/fold mindent
		if (isRegular(game) && betsize > 35 && maxStackSize > 120 && !(game.getHand().isPocket() && game.getHand().getCard1().getRank() == 'A'))
		{
			logger.logExp("\t\t\tModifying Raise: if NOT AA EQ -30%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.30);
		}

		//IMPLEMENT PREFLOP 4BET
		if (betsize > 30 && maxStackSize > 105 && !game.getHand().is100BBStackOff())
		{
			EQ = modifyValue(EQ, -0.20);
			logger.logExp("\t\t\tModifying Raise: STACKOFF 100BB WEAK HAND EQ -20%", BOT_LOGIC);
		}

		//DEEP STACK OFF
		if (betsize > 30 && effectiv > 200 && !(game.getHand().isPocket() && game.getHand().getCard1().getRank() == 'A'))
		{
			EQ = modifyValue(EQ, -0.20);
			logger.logExp("\t\t\tModifying Raise: STACKOFF 100BB WEAK HAND EQ -20%", BOT_LOGIC);
		}
	}
	else if (game.getStreet() == 1)
	{
		if (effectiv > 200)
		{
			EQ = modifyValue(EQ, -0.10);
		}

		if (effectiv > 250 && isRegular(game))
		{
			EQ = modifyValue(EQ, -0.10);
		}

		if (effectiv > 300)
		{
			EQ = modifyValue(EQ, -0.05);
		}

		if (effectiv > 400)
		{
			EQ = modifyValue(EQ, -0.05);
		}
	}
	else if (game.getStreet() == 2)
	{
		if (effectiv > 200)
		{
			EQ = modifyValue(EQ, -0.10);
		}

		if (effectiv > 250 && isRegular(game))
		{
			EQ = modifyValue(EQ, -0.10);
		}

		if (effectiv > 300)
		{
			EQ = modifyValue(EQ, -0.05);
		}

		if (effectiv > 400)
		{
			EQ = modifyValue(EQ, -0.05);
		}
	}
	else if (game.getStreet() == 3)
	{
		if (effectiv > 200)
		{
			EQ = modifyValue(EQ, -0.10);
		}

		if (effectiv > 250 && isRegular(game))
		{
			EQ = modifyValue(EQ, -0.10);
		}

		if (effectiv > 300)
		{
			EQ = modifyValue(EQ, -0.05);
		}

		if (effectiv > 400)
		{
			EQ = modifyValue(EQ, -0.05);
		}
	}

	return EQ;
}
double manipulateEQCall(double EQ, CurrentGameInfo& game)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);

	if (game.getStreet() == 0)
	{
		EQ = modifyValue(EQ, -0.05);
		logger.logExp("\t\t\tModifying Call: DEFAULT EQ -5%", BOT_LOGIC);

		if (game.getAmountToCall() + game.getHero().getBetsize() > 6)
		{
			EQ = modifyValue(EQ, -0.2);
			logger.logExp("\t\t\tModifying Call: > 6 bet call EQ -20%", BOT_LOGIC);
		}
		if (game.getAmountToCall() + game.getHero().getBetsize() > 20) 
		{
			logger.logExp("\t\t\tModifying Call: > 20 bet call EQ -30%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.3);
		}
		if (!heroInPosition(game)) 
		{
			logger.logExp("\t\t\tModifying Call: Not ins position EQ -10%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.1);
		}
	}
	else if (game.getStreet() == 1)
	{		
		//DEFAULT VACUUM
		EQ = modifyValue(EQ, -0.07);
		if (!heroInPosition(game))
		{
			EQ = modifyValue(EQ, -0.05);
		}

		if (!heroInPosition(game) &&  game.getPotcommon() > 20 && game.getBiggestBet() > game.getPotcommon())
		{
			EQ = modifyValue(EQ, -0.05);
		}

		int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());
		if (str > 2 && str < 6 && !heroInPosition(game) && game.getBiggestBet() > 0.1 * game.getPotcommon())
		{	
			logger.logExp("\t\t\tModifying Call: str > 2 && str < 7 -6%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.06);
		}

		if (str > 2 && str < 5 && game.getBiggestBet() > game.getPotcommon())
		{
			EQ = modifyValue(EQ, -0.05);
		}
		
		//OOP EQ--
		if (!heroInPosition(game) && game.getBiggestBet() > 0.2 * game.getPotcommon()) 
		{
			logger.logExp("\t\t\tModifying Call: Not in position EQ -10%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.06);
		}
	}
	else if (game.getStreet() == 2)
	{
		//DEFAULT VACUUM
		EQ = modifyValue(EQ, -0.05);
		logger.logExp("\t\t\tModifying Call: DEFAULT EQ -5%", BOT_LOGIC);
		int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());

		if (!heroInPosition(game))
		{
			EQ = modifyValue(EQ, -0.05);
		}

		if (!heroInPosition(game) &&  game.getPotcommon() > 45 && game.getBiggestBet() > game.getPotcommon())
		{
			EQ = modifyValue(EQ, -0.05);
		}
		
		if (str > 2 && str < 6 && !heroInPosition(game) && game.getBiggestBet() > 0.1 * game.getPotcommon())
		{	
			logger.logExp("\t\t\tModifying Call: str > 2 && str < 7 -10%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.06);
		}

		if (str > 2 && str < 5 && game.getBiggestBet() > game.getPotcommon())
		{
			EQ = modifyValue(EQ, -0.05);
		}

		//OOP EQ--
		if (!heroInPosition(game) && game.getBiggestBet() > 0.2 * game.getPotcommon()) 
		{
			logger.logExp("\t\t\tModifying Call: Not ins position EQ -5%", BOT_LOGIC);
			EQ = modifyValue(EQ, -0.04);
		}
	}
	else if (game.getStreet() == 3)
	{
		int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());

		if (game.getPotcommon() * 0.1 > game.getBiggestBet()) EQ = modifyValue(EQ, 0.1);
		EQ = modifyValue(EQ, -0.05);

		if (abs(game.getAmountToCall() - 0) < 0.001)
		{
			EQ = modifyValue(EQ, -1);
		}
	}

	return EQ;
}
double manipulateFE(double FE, double betsize, CurrentGameInfo& game, CurrentPlayerInfo& player)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);

	betsize /= game.getBblind();

	int boardType = Evaluator::boardType(game.getBoard());
	double potcommon = game.getPotcommon();

	if (game.getStreet() == 0)
	{
		//RANDOMIZE FE
		FE = modifyValue(FE, -0.10);

		//OOP FE-- (NO 3BET BLUFF OOP)
		if (!heroInPosition(game)) 
		{
			FE = modifyValue(FE, -0.10);
			logger.logExp("\t\t\tModifying FE: OOP 3bet -15%", BOT_LOGIC);
		}
	}
	else if (game.getStreet() == 1)
	{
		int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());

		FE = modifyValue(FE, -0.15);
		if (game.getOpponentsInfo().size() > 1 && str > 1) FE = modifyValue(FE, 0.1);
		logger.logExp("\t\t\tModifying FE: DEFAULT -15%", BOT_LOGIC);

		
		if (str > 2 && str < 5 && betsize > game.getPotcommon()) 
		{
			FE = modifyValue(FE, -0.5);
		}
		
		if (boardType == 1)
		{
			logger.logExp("\t\t\tModifying FE: BOARDTYPE == 1 -20%", BOT_LOGIC);
			FE = modifyValue(FE, -0.25);
		}
		else if (boardType == 2)
		{
			logger.logExp("\t\t\tModifying FE: BOARDTYPE == 2 -30%", BOT_LOGIC);
			
			if (heroInPosition(game)) 
			{
				FE = modifyValue(FE, -0.10);
				logger.logExp("\t\t\tModifying FE: BOARDTYPE == 2 -10%", BOT_LOGIC);
			}
			FE = modifyValue(FE, -0.35);
		}
	}
	else if (game.getStreet() == 2)
	{
		//DEFAULT SUBSTRACT FE
		FE = modifyValue(FE, -0.20);
		logger.logExp("\t\t\tModifying FE: DEFAULT -15%(20%)", BOT_LOGIC);

		if (game.getOpponentsInfo().size() == 1)
		{
			FE = modifyValue(FE, -0.05);
			logger.logExp("\t\t\tModifying FE: -5%", BOT_LOGIC);
		}

		if (boardType == 1)
		{
			logger.logExp("\t\t\tModifying FE: BOARDTYPE == 1 -10%", BOT_LOGIC);
			FE = modifyValue(FE, -0.10);
		}
		else if (boardType == 2)
		{
			logger.logExp("\t\t\tModifying FE: BOARDTYPE == 1 -20%", BOT_LOGIC);
			FE = modifyValue(FE, -0.20);
		}
		else if (boardType == 3)
		{
			logger.logExp("\t\t\tModifying FE: BOARDTYPE == 3 -25%", BOT_LOGIC);
			FE = modifyValue(FE, -0.25);
		}
	}
	else if (game.getStreet() == 3)
	{
		FE = modifyValue(FE, -0.20);
		logger.logExp("\t\t\tModifying FE: DEFAULT -10%(15%)", BOT_LOGIC);
		if (boardType == 2)
		{
			logger.logExp("\t\t\tModifying FE: BOARDTYPE == 2 -20%", BOT_LOGIC);
			FE = modifyValue(FE, -0.25);
		}
		else if (boardType == 3)
		{
			logger.logExp("\t\t\tModifying FE: BOARDTYPE == 3 -35%", BOT_LOGIC);
			FE = modifyValue(FE, -0.35);
		}
	}

	//===============================MANIPULATE BY BETSIZE=========================================
	double currentbet = player.getBetsize();
	int nBetsize = normalizeBetSize(game.getStreet() + 1, betsize, potcommon, game.getBblind());

	if (game.getStreet() == 0)
	{
		double ratio = betsize / currentbet;

		if (ratio < 2.9)
		{
			FE = modifyValue(FE, -0.10);
			
			logger.logExp("\t\t\tModifying FE: SMALL RAISE -10%", BOT_LOGIC);
		}
	}
	else
	{
		if (game.getStreet() == 1)
		{
			if (currentbet > potcommon * 0.3)
			{
				double ratio = betsize / currentbet;
				if (ratio < 2.9)
				{
					FE = modifyValue(FE, -0.2);
					logger.logExp("\t\t\tModifying FE: SMALL RAISE -20%", BOT_LOGIC);
				}
			}
			else
			{
				if (nBetsize == 0)
				{
					FE = modifyValue(FE, -0.25);
					logger.logExp("\t\t\tModifying FE: nBetsize == 0 -25%", BOT_LOGIC);
				}

				if (nBetsize == 1)
				{
					FE = modifyValue(FE, -0.15);
					logger.logExp("\t\t\tModifying FE: nBetsize == 1 -15%", BOT_LOGIC);
				}
			}
		}
		else if (game.getStreet() > 1)
		{
			if (currentbet > potcommon * 0.3)
			{
				double ratio = betsize / currentbet;
				if (ratio < 3)
				{
					FE = modifyValue(FE, -0.2);
					logger.logExp("\t\t\tModifying FE: SMALL RAISE -20%", BOT_LOGIC);
				}
			}
			else
			{
				if (nBetsize == 0)
				{
					FE = modifyValue(FE, -0.25);
					logger.logExp("\t\t\tModifying FE: nBetsize == 0 -25%", BOT_LOGIC);
				}

				if (nBetsize == 1)
				{
					FE = modifyValue(FE, -0.15);
					logger.logExp("\t\t\tModifying FE: nBetsize == 1 -15%", BOT_LOGIC);
				}
			}
		}
	}

	return FE;
}
//======================================================================================================

double BayesDecision::calculateEQ(vector<PlayerRange>& ranges, vector<Card>& board, Hand& hand, CurrentGameInfo& game)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);

	vector<PlayerRange> result;
	EqCalculator calc;
	PlayerRange myRange;
	myRange.range.insert(make_pair(hand, 1));

	result.push_back(myRange);

	logger.logExp("=============BEGIN EQ CALCULATION========================", RANGE_LOGGER);

	for (int i = 0; i < ranges.size(); ++i)
	{
		result.push_back(ranges[i]);
		logger.logExp("PLAYER NAME: " + game.getPlayerbyId(ranges[i].getId()).getName(), RANGE_LOGGER);
		logger.logExp("HAND: " + game.getHand().toString(), RANGE_LOGGER);
		logger.logExp("BOARD: ", RANGE_LOGGER);

		stringstream str;

		for (int j = 0; j < board.size(); ++j)
		{
			str << board[j].toString();
		}
		logger.logExp(str.str().c_str(), RANGE_LOGGER);

		logger.logExp("Total percentage of ranges: ", result[i + 1].totalPercentage(), BOT_LOGIC);
		logger.logExp(result[i + 1].toString(), RANGE_LOGGER);
		logger.logExp("=====================================================================", RANGE_LOGGER);
	}

	logger.logExp("Calculating EQ...", BOT_LOGIC);
	double eq = calc.calculate(result, board, 10000);
	logger.logExp("Calculated original EQ", eq, BOT_LOGIC);
	logger.logExp("=============END EQ CALCULATION========================", RANGE_LOGGER);
	//eq = modifyValue(eq, -0.05);
	return eq;
}
vector<double> BayesDecision::getFoldEquities(double betsize, CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn , BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	logger.logExp("\t============================CALCULATING FE's=====================", BOT_LOGIC);
	vector<double> result;

	if (game.getStreet() == 0)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
			double akt = preflop.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getStacksize() * game.getBblind(), player.getPoz(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);
			if (akt < 0) akt = 0;
			akt = manipulateFE(akt, betsize, game, player);
			result.push_back(akt);
		}
	}
	else if (game.getStreet() == 1)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
			
			double akt = flop.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);
			if (akt < 0) akt = 0;
			akt = manipulateFE(akt, betsize, game, player);
			result.push_back(akt);
		}
	}
	else if (game.getStreet() == 2)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
			double akt = turn.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), game.getFlopPotSize() * game.getBblind(), patternsNeeded);
			akt = manipulateFE(akt, betsize, game, player);
			result.push_back(akt);
		}
	}
	else if (game.getStreet() == 3)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
			double akt = river.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), game.getFlopPotSize() * game.getBblind(), patternsNeeded);
			if (akt < 0) akt = 0;
			akt = manipulateFE(akt, betsize, game, player);
			result.push_back(akt);
		}
	}

	logger.logExp("\t============================CALCULATED FE's=====================", BOT_LOGIC);
	for (int i = 0; i < result.size(); ++i)
	{
		logger.logExp("\t\tFE= ", result[i], BOT_LOGIC);
	}

	return result;
}
double BayesDecision::calculateEVRaise(CurrentGameInfo& gameInfo, vector<PlayerRange>& callRaiseRanges, vector<double>& foldEquities, double betsize, int& moveType, bool allIn = false)
{ 
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	logger.logExp("\t\t===================Calculating EV of raise with betsize========================", betsize, BOT_LOGIC);

	double ev = 0;

	int n = callRaiseRanges.size();

	double commonpot = gameInfo.getPotcommon() * gameInfo.getBblind();
	double heroCurrentBet = gameInfo.getHero().getBetsize() * gameInfo.getBblind();
	logger.logExp("\t\tCommonpot : ", commonpot, BOT_LOGIC);
	logger.logExp("\t\tHero currentbet : ", heroCurrentBet, BOT_LOGIC);

	if (allIn)
		logger.logExp("\t\tGOING ALLIN", BOT_LOGIC);

	double EQ = 0;

	for (int bits = 0; bits < (1 << n); ++bits)
	{
		vector<PlayerRange> actualRanges;
		vector<CurrentPlayerInfo> playersPlaying;
		double chanceOfThis = 1;
		double nr = 0;
		// Includes common pot + currentbet of players whose actions are fold
		double pot = commonpot;

		double totalCalls = 0;

		for (int i = 0; i < n; ++i)
		{
			if (bits & (1 << i))
			{
				chanceOfThis *= 1 - foldEquities[i];
				actualRanges.push_back(callRaiseRanges[i]);

				CurrentPlayerInfo player  = gameInfo.getPlayerbyId(callRaiseRanges[i].getId());
				double stacksize = player.getActualStacksize() + player.getBetsize();
				stacksize *= gameInfo.getBblind();
				if (stacksize < betsize)
					totalCalls += stacksize;
				else
					totalCalls += betsize;
				playersPlaying.push_back(player);
				nr++;
			}
			else
			{
				chanceOfThis *= foldEquities[i];
				CurrentPlayerInfo player  = gameInfo.getPlayerbyId(callRaiseRanges[i].getId());

				pot += player.getBetsize() * gameInfo.getBblind();
			}
		}

		double eq = 1;
		if (nr > 0) eq = calculateEQ(actualRanges, gameInfo.getBoard(), gameInfo.getHand(), gameInfo);
		logger.logExp("\t\tCalculated EQ : ", eq, BOT_LOGIC);
		//MANIPULATE EQ RAISE
		if (nr > 0 && !allIn) eq = manipulateEQRaise(eq, betsize, gameInfo);
		if (nr > 0 && allIn) eq = manipulateEQRaiseAllIn(eq, betsize, gameInfo);

		if (n == 1 && nr > 0) EQ = eq;

		logger.logExp("\t\tFinal EQ : ", eq, BOT_LOGIC);
		double thisEV = eq * (pot + totalCalls + heroCurrentBet) - (1 - eq) * (betsize - heroCurrentBet);
		thisEV -= (pot + totalCalls + betsize) * 0.05;

		logger.logExp("\t\tTotal calls : ", totalCalls, BOT_LOGIC);
		logger.logExp("\t\tChance of this situation : ", chanceOfThis, BOT_LOGIC);
		logger.logExp("\t\tEV of this situation : ", thisEV, BOT_LOGIC);

		ev += chanceOfThis * thisEV;
	}

	logger.logExp("\tCalculated EV : ", ev, BOT_LOGIC);

	//IMPLEMENT: IF NOT AI RANDOM RIVER BLUFF
	//IMPLEMENT: IF NOT AI RANDOM FLOP RAISE BLUFF
	if (n == 1 && ev > 0)
	{
		if (EQ > 0.5)
		{
			moveType = 0; //VBET
			logger.logExp("\tMoveType is VBET", BOT_LOGIC);
		}
		else if (EQ > 0.28)
		{
			moveType = 1; //SEMI-BLUFF
			logger.logExp("\tMoveType is SEMI BLUFF", BOT_LOGIC);
		}
		else
		{
			moveType = 2;
			logger.logExp("\tMoveType is BLUFF", BOT_LOGIC);
		}
	}

	return ev;
}
double BayesDecision::calculateEVCall(CurrentGameInfo& game, vector<PlayerRange>& ranges)
{
	Logger& logger = Logger::getLogger(LOGGER_TYPE::BOT_LOGIC);

	logger.logExp("\t=============================START CALL EV CALCULATION================================\n", LOGGER_TYPE::BOT_LOGIC);
	double EVCALL = 0;

	if (game.getStreet() == 0)
	{
		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		double eq = calculateEQ(ranges, game.getBoard(), game.getHand(), game);
		
		//MANIPULATE EQ CALL/CHECK
		eq = manipulateEQCall(eq, game);

		EVCALL = eq * totalpot - (1 - eq) * (call);
		EVCALL -= (totalpot + call) * 0.05;

		stringstream stream;
		stream << "\tTOTALPOT= " << totalpot << endl;
		stream << "\tCALL= " << call << endl;
		stream << "\tEQ= " << eq << endl;
		stream << "\tEVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}
	else if (game.getStreet() == 1)
	{

		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		double potcommon = game.getPotcommon() * game.getBblind();
		double eq = calculateEQ(ranges, game.getBoard(), game.getHand(), game);

		//MANIPULATE EQ CALL/CHECK
		eq = manipulateEQCall(eq, game);

		EVCALL = eq * totalpot - (1 - eq) * (call);
		EVCALL -= (totalpot + call) * 0.05;

		stringstream stream;
		stream << "\tTOTALPOT= " << totalpot << endl;
		stream << "\tCALL= " << call << endl;
		stream << "\tEQ= " << eq << endl;
		stream << "\tEVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}
	else if (game.getStreet() == 2)
	{

		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		double potcommon = game.getPotcommon() * game.getBblind();
		double eq = calculateEQ(ranges, game.getBoard(), game.getHand(), game);

		//MANIPULATE EQ CALL/CHECK
		eq = manipulateEQCall(eq, game);

		EVCALL = eq * totalpot - (1 - eq) * (call);
		EVCALL -= (totalpot + call) * 0.05;

		stringstream stream;
		stream << "\tTOTALPOT= " << totalpot << endl;
		stream << "\tCALL= " << call << endl;
		stream << "\tEQ= " << eq << endl;
		stream << "\tEVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}
	else if (game.getStreet() == 3)
	{
		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		double commonpot = game.getPotcommon() * game.getBblind();
		double eq = calculateEQ(ranges, game.getBoard(), game.getHand(), game);

		//MANIPULATE EQ CALL/CHECK
		eq = manipulateEQCall(eq, game);

		EVCALL = eq * totalpot - (1 - eq) * (call);
		EVCALL -= (totalpot + call) * 0.05;

		stringstream stream;
		stream << "\tTOTALPOT= " << totalpot << endl;
		stream << "\tCALL= " << call << endl;
		stream << "\tEQ= " << eq << endl;
		stream << "\tEVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}

	return EVCALL;
}
//======================================AI FUNCTIONS====================================================
bool BayesDecision::committed(CurrentGameInfo& game)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	logger.logExp("\tARE WE COMMITTED?", BOT_LOGIC);

	double totalpot = game.getTotalPot();
	double call = game.getAmountToCall();

	totalpot += call;

	double effectiv = game.getHero().getActualStacksize() - call;
	if (effectiv < 0) effectiv = 0;

	double opponentMaxStackSize = maxOpponentStack(game);

	logger.logExp("\tHero actualStack: ",  game.getHero().getActualStacksize(), BOT_LOGIC);
	logger.logExp("\tHero call", call, BOT_LOGIC);
	logger.logExp("\tHero effectiv stack: ", effectiv, BOT_LOGIC);
	logger.logExp("\tOpponent max stacksize: ", opponentMaxStackSize, BOT_LOGIC);

	if (effectiv > opponentMaxStackSize) effectiv = opponentMaxStackSize;

	double EQ = effectiv / (2 * effectiv + totalpot);

	logger.logExp("\tEQUITY NEEDED : ", EQ, BOT_LOGIC);
	
	if (EQ < 0.25)
	{
		return true;
	}

	return false;

}
bool BayesDecision::canCallAfterRaise(CurrentGameInfo& gameInfo, PlayerRange& range, double betsize, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn , BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);

	logger.logExp("\t===============BEGIN canCallAfterRaise================= ", BOT_LOGIC);

	PlayerRange allinRange;
	CurrentPlayerInfo& player = gameInfo.getPlayerbyId(range.getId());

	if (gameInfo.getStreet() == 1)
	{
		allinRange = flop.getRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * gameInfo.getBblind(), betsize, gameInfo.getBblind(), 
										gameInfo.getPotcommon() * gameInfo.getBblind(), gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);
	}
	else if (gameInfo.getStreet() == 2)
	{
		allinRange = turn.getRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * gameInfo.getBblind(), betsize, gameInfo.getBblind(), 
										gameInfo.getPotcommon() * gameInfo.getBblind(), gameInfo.getFlopPotSize() * gameInfo.getBblind(), gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);
	}
	else if (gameInfo.getStreet() == 3)
	{
		allinRange = river.getRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * gameInfo.getBblind(), betsize, gameInfo.getBblind(), 
										 gameInfo.getPotcommon() * gameInfo.getBblind(), gameInfo.getFlopPotSize() * gameInfo.getBblind(), gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);
	}

	logger.logExp("\t===============END of getting allinRange================= ", BOT_LOGIC);

	// If we have not enough patterns, we can let the bot go
	if (allinRange.range.size() == 0)
	{
		logger.logExp("\tERROR IN OPPONENT ALLIN RANGE : RETURNING TRUE", BOT_LOGIC);
		return true;
	}

	allinRange = RangeUtils::mergeRange(range, allinRange, gameInfo.getBoard(), gameInfo.getHand());
	allinRange.setId(range.getId());

	logger.logExp("\t===============END of merging allinRange================= ", BOT_LOGIC);

	vector<PlayerRange> ranges;
	ranges.push_back(allinRange);

	double EQ = calculateEQ(ranges, gameInfo.getBoard(), gameInfo.getHand(), gameInfo);
	EQ = manipulateEQRaiseAllIn(EQ, betsize, gameInfo);

	logger.logExp("\tEQUITY VS ALLIN RANGE : ", EQ, BOT_LOGIC);

	double originalpot = gameInfo.getTotalPot() * gameInfo.getBblind();
	double herocurrentbet = gameInfo.getHero().getBetsize() * gameInfo.getBblind();
	double opponentstack = gameInfo.getPlayerbyId(range.getId()).getActualStacksize() * gameInfo.getBblind();
	double opponentcurrentbet = gameInfo.getPlayerbyId(range.getId()).getBetsize() * gameInfo.getBblind();

	double effectivestack = (opponentstack + opponentcurrentbet);
	double herostack = (gameInfo.getHero().getActualStacksize() + gameInfo.getHero().getBetsize()) * gameInfo.getBblind();
	if (effectivestack > herostack)
		effectivestack = herostack;

	double totalpot = originalpot + betsize - herocurrentbet + opponentstack;
	double call = effectivestack - betsize;
	if (call < 0) call = 0;

	logger.logExp("\tTOTALPOT WHEN ALLIN CALL EV AFTER RAISING : ", totalpot, BOT_LOGIC);
	logger.logExp("\tCALL WHEN ALLIN CALL EV AFTER RAISING : ", call, BOT_LOGIC);
	logger.logExp("\tEQ WHEN ALLIN CALL EV AFTER RAISING : ", EQ, BOT_LOGIC);
	double evcall = EQ * totalpot - (1 - EQ) * call;
	evcall -= ( 2*effectivestack + gameInfo.getPotcommon() * gameInfo.getBblind() ) * 0.05;

	logger.logExp("\tALLIN CALL EV AFTER RAISING : ", evcall, BOT_LOGIC);

	return evcall > 0;
}

Action BayesDecision::makeDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(LOGGER_TYPE::BOT_LOGIC);
	srand(time(NULL));

	logger.logExp("====================Hand: " + game.getHand().toString(), LOGGER_TYPE::BOT_LOGIC );
	logger.logExp("Opponent number: ", (int)ranges.size(), BOT_LOGIC);
	if (ranges.size() == 0)
	{
		logger.logExp("Cannot decide action 1", LOGGER_TYPE::BOT_LOGIC);
		return Action('n', 0);
	}

	for (int i = 0; i < ranges.size(); ++i)
	{
		if (!ranges[i].getValid())
		{
			logger.logExp("Cannot decide action 2", LOGGER_TYPE::BOT_LOGIC);
			logger.logExp("Range id: ", ranges[i].getId(), BOT_LOGIC);
			return Action('n', 0);
		}
	}

	Action res;

	double EVRAISE = -100000;
	double maxRaiseSize = 0;
	int maxRaiseType = -1;
	double EVCALL = 0;

	bool bluffing = false;

	if (committed(game))
	{
		logger.logExp("===========COMMITTED================", BOT_LOGIC);
		double betsize = game.getHero().getActualStacksize() + game.getHero().getBetsize();
		double maxOpponent = maxOpponentStack(game) + game.getBiggestBet();
		if (betsize > maxOpponent) betsize = maxOpponent;
		betsize *= game.getBblind();

		vector<PlayerRange> aktRanges;
		
		for (int j = 0; j < ranges.size(); ++j)
		{
			PlayerRange akt;

			if (game.getPlayerbyId(ranges[j].getId()).getActualStacksize() > 0.01)
				akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
			else
				akt = ranges[j];

			if (akt.range.size() == 0) break;
			aktRanges.push_back(akt);
		}

		if (aktRanges.size() != ranges.size()) return Action('n', 0);
		
		vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);

		for (int i = 0; i < foldEquities.size(); ++i)
			foldEquities[i] *= 0.2;

		int moveType;
		double EVAI = calculateEVRaise(game, aktRanges, foldEquities, betsize, moveType, true);
		
		logger.logExp("EV of allin : ", EVAI, BOT_LOGIC);

		
		betsize = game.getHero().getActualStacksize() + game.getHero().getBetsize();
		betsize *= game.getBblind();
		if (EVAI > 0)
		{
			return Action('r', betsize);
		}
		
		return Action('f', 0);
	}
	
	vector<double> betSizes = getRaiseSizes(game);

	if (game.getStreet() == 0)
	{
		logger.logExp("==============PREFLOP CALCULATION STARTED==================", LOGGER_TYPE::BOT_LOGIC);

		for (int i = ranges.size() > 2 ? betSizes.size() - 1 : 0; i < betSizes.size(); ++i)
		{

			double betsize = betSizes[i];

			logger.logExp("actualBetSize: ", betSizes[i], LOGGER_TYPE::BOT_LOGIC);

			//STACK functions are FUCKED UP
			double stacksize = game.getHero().getActualStacksize() + game.getHero().getBetsize();
			double maxOpponentS = maxOpponentTotalStack(game);
			double effectiv = (stacksize < maxOpponentS ? stacksize : maxOpponentS);

			logger.logExp("\t\tHero actualStackSize: ", game.getHero().getActualStacksize(), BOT_LOGIC);
			logger.logExp("\t\tHeroCurrentBet: ", game.getHero().getBetsize(), BOT_LOGIC);
			logger.logExp("\t\tOpponent maxStack: ", maxOpponentS, BOT_LOGIC);
			logger.logExp("\t\teffective stack ", effectiv, BOT_LOGIC);
			logger.logExp("\t\tHero's stacksize : ", stacksize, BOT_LOGIC);

			effectiv *= game.getBblind();
			bool allIn = false;
			if (betsize >= 0.4 * effectiv)
			{
				betsize = effectiv;
				allIn = true;
			}

			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}

			if (aktRanges.size() != ranges.size()) break;
								
			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);

			int moveType;
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize, moveType, allIn);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
				maxRaiseType = moveType;
			}
		}

		EVCALL = calculateEVCall(game, ranges);

		if (EVRAISE < 0 && game.getHand().isPocket())
		{	
			logger.logExp("Calculate set call", BOT_LOGIC);
			double maxstacksize = 0;
			for (int i = 0; i < game.getOpponentsInfo().size(); ++i)
			{
				CurrentPlayerInfo& player = game.getOpponentsInfo()[i];
				if (abs(player.getBetsize()-game.getBiggestBet() < 0.01) && player.getStacksize() > maxstacksize)
				{
					maxstacksize = player.getStacksize();
				}
			}
			
			logger.logExp("MaxStackSize= ", maxstacksize, BOT_LOGIC);
			if (maxstacksize > game.getHero().getStacksize())
			{
				maxstacksize = game.getHero().getStacksize();
			}
			logger.logExp("MaxStackSizeHero= ", maxstacksize, BOT_LOGIC);

			if (maxstacksize > game.getAmountToCall() * 24)
			{
				EVCALL = 100000;
			}
		}

		if (EVRAISE == -100000 && EVCALL < 0)
		{
			logger.logExp("Cannot decide action EVRAISE == -10000", LOGGER_TYPE::BOT_LOGIC);
			return Action('n', 0);
		}
	}

	if (game.getStreet() == 1)
	{
		logger.logExp("===================FLOP CALCULATION STARTED============================", LOGGER_TYPE::BOT_LOGIC);

		int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());

		for (int i = ranges.size() > 2 ? betSizes.size() - 1 : 0; i < betSizes.size(); ++i)
		{

			double betsize = betSizes[i];
			double potcommon = game.getPotcommon() * game.getBblind();

			logger.logExp("actualBetSize: ", betSizes[i], LOGGER_TYPE::BOT_LOGIC);


			//STACK functions are FUCKED UP
			double stacksize = game.getHero().getActualStacksize() + game.getHero().getBetsize();
			double maxOpponentS = maxOpponentTotalStack(game);
			double effectiv = (stacksize < maxOpponentS ? stacksize : maxOpponentS);

			logger.logExp("\t\tHero actualStackSize: ", game.getHero().getActualStacksize(), BOT_LOGIC);
			logger.logExp("\t\tHeroCurrentBet: ", game.getHero().getBetsize(), BOT_LOGIC);
			logger.logExp("\t\tOpponent maxStack: ", maxOpponentS, BOT_LOGIC);
			logger.logExp("\t\teffective stack ", effectiv, BOT_LOGIC);
			logger.logExp("\t\tHero's stacksize : ", stacksize, BOT_LOGIC);

			effectiv *= game.getBblind();
			bool allIn = false;
			if (betsize >= 0.4 * effectiv)
			{
				betsize = effectiv;
				allIn = true;
			}

			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0)
				{
					logger.logExp("Couldn't get opponent's calling+raising range.", LOGGER_TYPE::BOT_LOGIC);
					break;
				}
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) break;

			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);
			
			int moveType;
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize, moveType, allIn);

			if (ranges.size() == 1 && betsize > potcommon && str <= 2 && str != 0 && !canCallAfterRaise(game, ranges[0], betsize, preflop, flop, turn, river))
				evraise = -10000;


			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
				maxRaiseType = moveType;
			}
		}

		//IMPLEMENT: CHECK OOP VS REGULAR
		EVCALL = calculateEVCall(game, ranges);
		double potcommon = game.getPotcommon() * game.getBblind();

		//IP VISZONYLAG SZARAZ BOARDON EROS HAND
		int boardType = Evaluator::boardType(game.getBoard());
		if (ranges.size() == 1 && EVRAISE > 0 && boardType < 3 && maxRaiseType == 0 && heroInPosition(game) && game.getBiggestBet() * game.getBblind() <= potcommon 
			&& game.getBiggestBet() * game.getBblind()> potcommon * 0.5)
		{
			CurrentPlayerInfo& player = game.getPlayerbyId(ranges[0].getId());
			double AF = player.getAF();
			//SLOWPLAY
			if (AF >= 3)
			{
				EVCALL = 100000;
				logger.logExp("===================SLOWPLAYING HAND============================", LOGGER_TYPE::BOT_LOGIC);
			}
		}

		//RANDOM ODA CHECK REGULARNAK
		int random = rand() % 3;
		if (ranges.size() == 1 && isRegular(game) && !heroInPosition(game) && game.getHand().isPocket() && !game.getHand().getCard1().isBroadway() && boardType < 3 && (str == 0 || str == 3) &&
			((game.getHero().getPoz() == 1 && game.getPlayerbyId(ranges[0].getId()).getPoz() != 2) || game.getHero().getPoz() == 2))
		{
			logger.logExp("======================POCKET OOP REGULAR CHECK===================", LOGGER_TYPE::BOT_LOGIC);
			EVRAISE = -1000;
		}
		else if (random == 0 && ranges.size() == 1 && !heroInPosition(game) && isRegular(game) && abs(game.getBiggestBet() - 0) < 0.01 && maxRaiseType == 0 && boardType < 3)
		{
			logger.logExp("======================RANDOM REGULAR CHECK===================", LOGGER_TYPE::BOT_LOGIC);
			EVRAISE = -1000;
		}

		//PLAY VS AGGRO CHBH
		if (ranges.size() == 1 && heroInPosition(game) && game.getBiggestBet() * game.getBblind() < potcommon * 0.2 && maxRaiseType == 0 && boardType < 3 && isRegular(game))
		{
			int random;
			random = random % 3;
			if (str == 0) random = 1;//nem chbh

			CurrentPlayerInfo& player = game.getPlayerbyId(ranges[0].getId());
			double AF = player.getAF();

			if (AF >= 2.5 && random == 0)
			{
				EVCALL = 100000;
				logger.logExp("=====================CHBH VS AGGRO ============================", LOGGER_TYPE::BOT_LOGIC);
			}
		}

		if (ranges.size() == 1 && maxRaiseType == 2 && !heroInPosition(game))
		{
			EVRAISE = -1000;
			logger.logExp("YOU DON'T LET ME BLUFF!!", LOGGER_TYPE::BOT_LOGIC);
		}

		if (EVRAISE == -100000 && EVCALL < 0)
		{
			logger.logExp("Cannot decide action EVRAISE == -10000", LOGGER_TYPE::BOT_LOGIC);
			return Action('n', 0);
		}
	}

	if (game.getStreet() == 2)
	{
		logger.logExp("=========================TURN CALCULATION STARTED==========================", LOGGER_TYPE::BOT_LOGIC);

		int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());

		for (int i = ranges.size() > 2 ? betSizes.size() - 1 : 0; i < betSizes.size(); ++i)
		{

			double betsize = betSizes[i];

			double potcommon = game.getPotcommon() * game.getBblind();

			logger.logExp("actualBetSize: ", betSizes[i], LOGGER_TYPE::BOT_LOGIC);

						//STACK functions are FUCKED UP
			double stacksize = game.getHero().getActualStacksize() + game.getHero().getBetsize();
			double maxOpponentS = maxOpponentTotalStack(game);
			double effectiv = (stacksize < maxOpponentS ? stacksize : maxOpponentS);

			logger.logExp("\t\tHero actualStackSize: ", game.getHero().getActualStacksize(), BOT_LOGIC);
			logger.logExp("\t\tHeroCurrentBet: ", game.getHero().getBetsize(), BOT_LOGIC);
			logger.logExp("\t\tOpponent maxStack: ", maxOpponentS, BOT_LOGIC);
			logger.logExp("\t\teffective stack ", effectiv, BOT_LOGIC);
			logger.logExp("\t\tHero's stacksize : ", stacksize, BOT_LOGIC);

			effectiv *= game.getBblind();
			bool allIn = false;
			if (betsize >= 0.4 * effectiv)
			{
				betsize = effectiv;
				allIn = true;
			}

			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) break;
		
			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);
			
			int moveType;
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize, moveType, allIn);

			if (ranges.size() == 1 && betsize > potcommon && str <= 2 && !canCallAfterRaise(game, ranges[0], betsize, preflop, flop, turn, river))
				evraise = -10000;

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
				maxRaiseType = moveType;
			}
		}

		//IMPLEMENT: OOP BET INSTEAD CHECK?
		EVCALL = calculateEVCall(game, ranges);

		double potcommon = game.getPotcommon() * game.getBblind();
		if (ranges.size() == 1 && EVRAISE > 0 && maxRaiseType == 2 && maxRaiseSize > potcommon)
		{
			logger.logExp("======================NOT LETTING RAISE BLUFF===================", LOGGER_TYPE::BOT_LOGIC);
			EVRAISE = -100;
		}

		//RANDOM ODA CHECK REGULARNAK
		int random = rand() % 3;
		int boardType = Evaluator::boardType(game.getBoard());
		if (random == 0 && ranges.size() == 1 && !heroInPosition(game) && isRegular(game) && abs(game.getBiggestBet() - 0) < 0.01 && maxRaiseType == 0 && boardType < 3)
		{
			logger.logExp("======================RANDOM REGULAR CHECK===================", LOGGER_TYPE::BOT_LOGIC);
			EVRAISE = -1000;
		}

		//IP VISZONYLAG SZARAZ BOARDON EROS HAND
		if (ranges.size() == 1 && EVRAISE > 0 && boardType < 3 && maxRaiseType == 0 && heroInPosition(game) && game.getBiggestBet() * game.getBblind() <= potcommon 
			&& game.getBiggestBet() * game.getBblind() > potcommon * 0.5)
		{
			CurrentPlayerInfo& player = game.getPlayerbyId(ranges[0].getId());
			double AF = player.getAF();
			//SLOWPLAY
			if (AF >= 3)
			{
				EVCALL = 100000;
				logger.logExp("===================SLOWPLAYING HAND============================", LOGGER_TYPE::BOT_LOGIC);
			}
		}

		//PLAY VS AGGRO CHBH
		if (ranges.size() == 1 && heroInPosition(game) && game.getBiggestBet() * game.getBblind() < potcommon * 0.2 && maxRaiseType < 2 && boardType < 3 && isRegular(game))
		{
			int random;	
			random = rand() % 2;

			if (str == 0) random = 1; //nem chbh eros hand!

			CurrentPlayerInfo& player = game.getPlayerbyId(ranges[0].getId());
			double AF = player.getAF();

			if (AF >= 2.5 && random == 0)
			{
				EVCALL = 100000;
				logger.logExp("=====================CHBH VS AGGRO ============================", LOGGER_TYPE::BOT_LOGIC);
			}
		}

		if (maxRaiseType == 2)
		{
			EVRAISE = -1000;
			logger.logExp("YOU DON'T LET ME BLUFF!!", LOGGER_TYPE::BOT_LOGIC);
		}


		if (EVRAISE == -100000 && EVCALL < 0) return Action('n', 0);
	}

	if (game.getStreet() == 3)
	{
		logger.logExp("==============================RIVER CALCULATION STARTED===================================", LOGGER_TYPE::BOT_LOGIC);

		int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());

		for (int i = ranges.size() > 2 ? betSizes.size() - 2 : 0; i < betSizes.size(); ++i)
		{
			double betsize = betSizes[i];
			double potcommon = game.getPotcommon() * game.getBblind();

			logger.logExp("actualBetSize: ", betSizes[i], LOGGER_TYPE::BOT_LOGIC);

						//STACK functions are FUCKED UP
			double stacksize = game.getHero().getActualStacksize() + game.getHero().getBetsize();
			double maxOpponentS = maxOpponentTotalStack(game);
			double effectiv = (stacksize < maxOpponentS ? stacksize : maxOpponentS);

			logger.logExp("\t\tHero actualStackSize: ", game.getHero().getActualStacksize(), BOT_LOGIC);
			logger.logExp("\t\tHeroCurrentBet: ", game.getHero().getBetsize(), BOT_LOGIC);
			logger.logExp("\t\tOpponent maxStack: ", maxOpponentS, BOT_LOGIC);
			logger.logExp("\t\teffective stack ", effectiv, BOT_LOGIC);
			logger.logExp("\t\tHero's stacksize : ", stacksize, BOT_LOGIC);

			effectiv *= game.getBblind();
			
			
			bool allIn = false;
			if (betsize >= 0.8 * effectiv)
			{
				betsize = effectiv;
				allIn = true;
			}

			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) break;
			
			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);
			
			int moveType;
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize, moveType, allIn);

			if (ranges.size() == 1 && betsize > potcommon && str <= 2 && !canCallAfterRaise(game, ranges[0], betsize, preflop, flop, turn, river))
				evraise = -10000;

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
				maxRaiseType = moveType;
			}
		}

		//IMPLEMENT: OOP BET INSTEAD CHECK
		EVCALL = calculateEVCall(game, ranges);

		double potcommon = game.getPotcommon() * game.getBblind();

		if (ranges.size() == 1 && maxRaiseType == 1)
		{
			EVRAISE = -1000;
			logger.logExp("YOU DON'T LET ME SEMI-BLUFF!!", LOGGER_TYPE::BOT_LOGIC);
		}

		if (maxRaiseType == 2)
		{
			EVRAISE = -1000;
			logger.logExp("YOU DON'T LET ME BLUFF!!", LOGGER_TYPE::BOT_LOGIC);
		}

		if (EVRAISE == -100000 && EVCALL < 0) return Action('n', 0);
	}

	double totalpot = game.getTotalPot() * game.getBblind();

	logger.logExp("DECIDED ACTION\n", LOGGER_TYPE::BOT_LOGIC);
	if (EVRAISE > 0 || EVCALL > 0)
	{
		if (abs(EVRAISE - EVCALL) <= game.getBblind() && EVRAISE > 0)
		{
			EVRAISE = 100000;
		}

		if (EVRAISE > EVCALL)
		{
			if (EVRAISE == -100000)
			{
				logger.logExp("CANNOT CALCULATE DECISION", BOT_LOGIC);
				res.setAction('n', 0);
			}
			else
			{
				
				logger.logExp("First MaxRaiseSize= ", maxRaiseSize, BOT_LOGIC);
				
				double opponentStack = maxOpponentTotalStack(game) * game.getBblind();
				double heroStack = (game.getHero().getActualStacksize() + game.getHero().getBetsize()) * game.getBblind();

				if (abs(maxRaiseSize - opponentStack) < 0.01 && heroStack > opponentStack)
				{
					maxRaiseSize = heroStack;
				}
				if (maxRaiseSize > heroStack) maxRaiseSize = heroStack;


				logger.logExp("MaxRaiseSize= ", maxRaiseSize, BOT_LOGIC);

				res.setAction('r', maxRaiseSize);
				if (bluffing) res.setBluff(true);
			}
		}
		else
		{
			if (EVRAISE == -100000)
			{
				logger.logExp("ACTION is hack\n", BOT_LOGIC);
				res.setAction('h', 0);
			}
			else if (abs(game.getAmountToCall() - 0) < 0.001)
			{
				res.setAction('x', 0);
			}
			else
			{
				res.setAction('c', 0);
			}
		}
	}
	else
	{
		res.setAction('f', 0);
	}

	return res;
};