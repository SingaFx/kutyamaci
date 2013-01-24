#include "bayesDecision.h"
#include "eqcalculator.h"
#include "logger.h"
#include "evaluator.h"
#include <sstream>

PlayerRange BayesDecision::getCallRaiseRange(double betsize, PlayerRange& range, CurrentGameInfo& game, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	PlayerRange totalRange;

	CurrentPlayerInfo player = game.getPlayerbyId(range.getId());

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
			game.getPotcommon() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
	}

	if (game.getStreet() == 3)
	{
		totalRange = river.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 0, betsize, game.getBblind(),
			game.getPotcommon() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
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

double BayesDecision::calculateEQ(vector<PlayerRange>& ranges, vector<Card>& board, Hand& hand)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);

	vector<PlayerRange> result;
	EqCalculator calc;
	PlayerRange myRange;
	myRange.range.insert(make_pair(hand, 1));

	result.push_back(myRange);

	for (int i = 0; i < ranges.size(); ++i)
	{
		result.push_back(ranges[i]);
		logger.logExp("Total percentage of ranges: ", result[i + 1].totalPercentage(), BOT_LOGIC);
		logger.logExp(result[i + 1].toString(), BOT_LOGIC);
	}

	logger.logExp("Calculating EQ...", BOT_LOGIC);

	return calc.calculate(result, board, 15000) / 100;
}
char BayesDecision::calculateDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	char res = 'n';

	return res;
}

double BayesDecision::modifyValue(double value, double change)
{
	value += change;
	if (value < 0) value = 0;
	if (value > 1) value = 1;

	return value;
}
double BayesDecision::modifyFEbyBetSize(int street, CurrentPlayerInfo& player, double betsize, double potcommon, double FE, double bblind)
{
	double currentbet = player.getBetsize();
	int nBetsize = normalizeBetSize(street, betsize, potcommon, bblind);

	if (street == 1)
	{
		double ratio = betsize / currentbet;

		if (ratio < 2.5)
		{
			FE = modifyValue(FE, -0.2);
		}

		if (ratio > 5)
		{
			FE = modifyValue(FE, 0.05);
		}
	}
	else
	{
		if (currentbet > potcommon * 0.5)
		{
			double ratio = betsize / currentbet;
			if (ratio < 2.5)
			{
				FE = modifyValue(FE, -0.15);
			}
			else if (ratio < 3.5)
			{
			}
			else if (ratio < 5)
			{
				FE = modifyValue(FE, 0.04);
			}
			else
			{
				FE = modifyValue(FE, 0.07);
			}
		}
		else
		{
			if (nBetsize == 0)
			{
				FE = modifyValue(FE, -0.15);
			}

			if (nBetsize == 1)
			{
				FE = modifyValue(FE, -0.05);
			}

			if (nBetsize == 2)
			{
			}

			if (nBetsize == 3)
			{
				FE = modifyValue(FE, 0.03);
			}

			if (nBetsize == 4)
			{
				FE = modifyValue(FE, 0.06);
			}

			if (nBetsize > 4)
			{
				FE = modifyValue(FE, 0.09);
			}
		}
	}

	return FE;
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
double BayesDecision::modifyFEbyPlayersInPlay(int number, double FE)
{
	if (number == 2)
	{
		FE = modifyValue(FE, 0.05);
	}
	else if (number > 2)
	{
		FE = modifyValue(FE, 0.1);
	}
	return FE;
}
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

double BayesDecision::modifyFEbyRelativePosition(CurrentGameInfo& gameInfo, double FE)
{
	Logger& logger = logger.getLogger(BOT_LOGIC);
	logger.logExp("Started modifyFEbyRelativePosition\n", BOT_LOGIC);
	
	int myPoz = gameInfo.getHero().getPoz();
	logger.logExp("My position is: ", myPoz, BOT_LOGIC);

	vector<CurrentPlayerInfo>& players = gameInfo.getOpponentsInfo();

	bool inPoz = true;
	for (int i = 0; i < players.size(); ++i)
	{
		int poz = players[i].getPoz();
		if (!inPosition(myPoz, poz))
		{
			inPoz = false;
		}
	}


	if (inPoz)
	{
		logger.logExp("In position\n", BOT_LOGIC);
		if (gameInfo.getStreet() == 0)
		{
			FE = modifyValue(FE, 0.05);
		}
		if (gameInfo.getStreet() == 1)
		{
			FE = modifyValue(FE, 0.1);
		}
		if (gameInfo.getStreet() == 2)
		{
			FE = modifyValue(FE, 0.05);
		}
	}
	else
	{
		logger.logExp("Not in position\n", BOT_LOGIC);
		if (gameInfo.getStreet() == 0)
		{
			FE = modifyValue(FE, -0.1);
		}
		if (gameInfo.getStreet() == 1)
		{
			FE = modifyValue(FE, -0.1);
		}
		if (gameInfo.getStreet() == 2)
		{
			FE = modifyValue(FE, -0.05);
		}
	}

	return FE;
}
double BayesDecision::modifyEQbyRelativePosition(CurrentGameInfo& gameInfo, vector<CurrentPlayerInfo>& players, double EQ)
{
	Logger& logger = logger.getLogger(BOT_LOGIC);
	logger.logExp("Started modifyEQbyRelativePosition\n", BOT_LOGIC);
	
	int myPoz = gameInfo.getHero().getPoz();
	logger.logExp("My position is: ", myPoz, BOT_LOGIC);

	bool inPoz = true;
	for (int i = 0; i < players.size(); ++i)
	{
		int poz = players[i].getPoz();
		if (!inPosition(myPoz, poz))
		{
			inPoz = false;
		}
	}

	if (inPoz)
	{
		logger.logExp("In position\n", BOT_LOGIC);
		if (gameInfo.getStreet() == 0)
		{
			EQ = modifyValue(EQ, 0.05);
		}
		if (gameInfo.getStreet() == 1)
		{
			EQ = modifyValue(EQ, 0.03);
		}
	}
	else
	{
		logger.logExp("Not in position\n", BOT_LOGIC);
		if (gameInfo.getStreet() == 0)
		{
			EQ = modifyValue(EQ, -0.1);
		}
		if (gameInfo.getStreet() == 1)
		{
			EQ = modifyValue(EQ, -0.08);
		}
		if (gameInfo.getStreet() == 2)
		{
			EQ = modifyValue(EQ, -0.05);
		}
	}

	return EQ;
}
//TODO: modify FE by board

vector<double> BayesDecision::getFoldEquities(double betsize, CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn , BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	logger.logExp("============================CALCULATING FE's=====================", BOT_LOGIC);
	vector<double> result;

	if (game.getStreet() == 0)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());

			double akt = preflop.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getStacksize() * game.getBblind(), player.getPoz(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);
			logger.logExp("Player's original FE : ", akt, BOT_LOGIC);
			akt = modifyFEbyBetSize(1, player, betsize / game.getBblind(), game.getPotcommon(), akt, game.getBblind());
			logger.logExp("Player's modified FE by betsize : ", akt, BOT_LOGIC);
			akt = modifyFEbyRelativePosition(game, akt);
			logger.logExp("Player's modified FE by relative position : ", akt, BOT_LOGIC);
			
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
			logger.logExp("Player's original FE : ", akt, BOT_LOGIC);
			akt = modifyFEbyBetSize(2, player, betsize / game.getBblind(), game.getPotcommon(), akt, game.getBblind());
			akt = modifyFEbyPlayersInPlay(ranges.size(), akt);
			akt = modifyFEbyRelativePosition(game, akt);

			result.push_back(akt);
		}
	}
	else if (game.getStreet() == 2)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
			double akt = turn.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);
			logger.logExp("Player's original FE : ", akt, BOT_LOGIC);
			akt = modifyFEbyBetSize(2, player, betsize / game.getBblind(), game.getPotcommon(), akt, game.getBblind());
			akt = modifyFEbyPlayersInPlay(ranges.size(), akt);
			akt = modifyFEbyRelativePosition(game, akt);

			result.push_back(akt);
		}
	}
	else if (game.getStreet() == 3)
	{
		for (int j = 0; j < ranges.size(); ++j)
		{
			CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
			double akt = river.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(),
				player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);
			logger.logExp("Player's original FE : ", akt, BOT_LOGIC);
			akt = modifyFEbyBetSize(2, player, betsize / game.getBblind(), game.getPotcommon(), akt, game.getBblind());
			akt = modifyFEbyPlayersInPlay(ranges.size(), akt);

			result.push_back(akt);
		}
	}

	logger.logExp("============================CALCULATED FE's=====================", BOT_LOGIC);
	for (int i = 0; i < result.size(); ++i)
	{
		logger.logExp("FE= ", result[i], BOT_LOGIC);
	}

	return result;
}

double BayesDecision::calculateEVRaise(CurrentGameInfo& gameInfo, vector<PlayerRange>& callRaiseRanges, vector<double>& foldEquities, double betsize)
{ 
	Logger& logger = Logger::getLogger(BOT_LOGIC);
	logger.logExp("===================Calculating EV of raise with betsize ", betsize, BOT_LOGIC);

	double ev = 0;

	int n = callRaiseRanges.size();

	double commonpot = gameInfo.getPotcommon() * gameInfo.getBblind();
	double heroCurrentBet = gameInfo.getHero().getBetsize() * gameInfo.getBblind();
	logger.logExp("Commonpot : ", commonpot, BOT_LOGIC);
	logger.logExp("Hero currentbet : ", heroCurrentBet, BOT_LOGIC);

	for (int bits = 0; bits < (1 << n); ++bits)
	{
		vector<PlayerRange> actualRanges;
		vector<CurrentPlayerInfo> playersPlaying;
		double chanceOfThis = 1;
		double nr = 0;
		// Includes common pot + currentbet of players whose actions are fold
		double pot = commonpot;

		for (int i = 0; i < n; ++i)
		{
			if (bits & (1 << i))
			{
				chanceOfThis *= 1 - foldEquities[i];
				actualRanges.push_back(callRaiseRanges[i]);

				nr++;
			}
			else
			{
				chanceOfThis *= foldEquities[i];
				CurrentPlayerInfo player  = gameInfo.getPlayerbyId(callRaiseRanges[i].getId());
				pot += player.getBetsize() * gameInfo.getBblind();
				playersPlaying.push_back(player);
			}
		}

		double eq = calculateEQ(actualRanges, gameInfo.getBoard(), gameInfo.getHand());
		logger.logExp("Calculated EQ : ", eq, BOT_LOGIC);
		if (nr > 0) eq = modifyEQbyRelativePosition(gameInfo, playersPlaying, eq);
		logger.logExp("Modified EQ : ", eq, BOT_LOGIC);

		// TODO : Include players who already folded ??
		double thisEV = eq * (pot + nr * betsize + heroCurrentBet) - (1 - eq) * (betsize - heroCurrentBet);

		logger.logExp("Chance of this situation : ", chanceOfThis, BOT_LOGIC);
		logger.logExp("EV of this situation : ", thisEV, BOT_LOGIC);

		ev += chanceOfThis * thisEV;
	}

	logger.logExp("===============Calculated EV : ", ev, BOT_LOGIC);

	return ev;
}

Action BayesDecision::makeDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(LOGGER_TYPE::BOT_LOGIC);

	logger.logExp("Hand: " + game.getHand().toString(), LOGGER_TYPE::BOT_LOGIC);
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

	double EVCALL = 0;

	if (game.getStreet() == 0)
	{
		logger.logExp("PREFLOP CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);
		int aktBetSize = normalizeBetSize(1, game.getBiggestBet() * game.getBblind(), game.getPotcommon() * game.getBblind(), game.getBblind()); 

		for (int i = aktBetSize + 1; i < preflop.PREFLOP_PLAYER_BET_SIZE_NUM && i < aktBetSize + 3; ++i)
		{
			double betsize = getBetsize(1, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			double maxraise = getMaxRaise(game) * game.getBblind();
			if (maxraise * 2 > betsize) betsize = maxraise * 2;

			logger.logExp("actualBetSize: ", i, LOGGER_TYPE::BOT_LOGIC);

			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}

			if (aktRanges.size() != ranges.size()) break;
								
			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
			}
		}

		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		
		logger.logExp("START CALL EV CALCULATION\n", LOGGER_TYPE::BOT_LOGIC);

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		eq = modifyEQbyRelativePosition(game, game.getOpponentsInfo(), eq);
		if (game.getAmountToCall() > 6) eq = modifyValue(eq, -0.15); 
		
		EVCALL = eq * totalpot - (1 - eq) * (call);

		stringstream stream;
		stream << "TOTALPOT= " << totalpot << endl;
		stream << "CALL= " << call << endl;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

		if (EVRAISE == -100000 && EVCALL < 0)
		{
			logger.logExp("Cannot decide action EVRAISE == -10000", LOGGER_TYPE::BOT_LOGIC);
			return Action('n', 0);
		}
	}

	if (game.getStreet() == 1)
	{
		logger.logExp("FLOP CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);

		int aktBetSize = normalizeBetSize(game.getStreet() + 1, game.getBiggestBet() * game.getBblind(), game.getPotcommon() * game.getBblind(), game.getBblind());

		for (int i = aktBetSize + 1; i < flop.PLAYER_BET_SIZE_NUM && i < aktBetSize + 4; ++i)
		{
			double betsize = getBetsize(2, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			double maxraise = getMaxRaise(game) * game.getBblind();
			if (maxraise * 2 > betsize) betsize = maxraise * 2;

			logger.logExp("actualBetSize: ", i, LOGGER_TYPE::BOT_LOGIC);
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
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = getBetsize(game.getStreet() + 1, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			}
		}

		logger.logExp("START CALL EV CALCULATION\n", LOGGER_TYPE::BOT_LOGIC);

		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		double potcommon = game.getPotcommon() * game.getBblind();

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		eq = modifyEQbyRelativePosition(game, game.getOpponentsInfo(), eq);

		if (game.getBiggestBet() > potcommon) eq = modifyValue(eq, -0.15); 
		else 
		{
			int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());
			if (str > 1 && str < 6)
			{			
				eq = modifyValue(eq, -0.1);
			}
		}
		EVCALL = eq * totalpot - (1 - eq) * (call);

		stringstream stream;
		stream << "TOTALPOT= " << totalpot << endl;
		stream << "CALL= " << call << endl;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

		
		if (EVRAISE == -100000 && EVCALL < 0)
		{
			logger.logExp("Cannot decide action EVRAISE == -10000", LOGGER_TYPE::BOT_LOGIC);
			return Action('n', 0);
		}

	}

	if (game.getStreet() == 2)
	{
		logger.logExp("TURN CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);

		int aktBetSize = normalizeBetSize(game.getStreet() + 1, game.getBiggestBet() * game.getBblind(), game.getPotcommon() * game.getBblind(), game.getBblind());

		for (int i = aktBetSize + 1; i < turn.PLAYER_BET_SIZE_NUM && i < aktBetSize + 4; ++i)
		{
			double betsize = getBetsize(2, i, game.getPotcommon() * game.getBblind(), game.getBblind());

			double maxraise = getMaxRaise(game) * game.getBblind();
			if (maxraise * 2 > betsize) betsize = maxraise * 2;

			logger.logExp("actualBetSize: ", i, LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) break;
		
			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
			}
		}

		logger.logExp("START CALL EV CALCULATION\n", LOGGER_TYPE::BOT_LOGIC);

		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		double potcommon = game.getPotcommon() * game.getBblind();

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		eq = modifyEQbyRelativePosition(game, game.getOpponentsInfo(), eq);

		if (game.getBiggestBet() > potcommon) eq = modifyValue(eq, -0.1); 
		else 
		{
			int str = Evaluator::cardStrength(game.getHand().getCard1(), game.getHand().getCard2(), game.getBoard());
			if (str > 1 && str < 6)
			{			
				eq = modifyValue(eq, -0.05);
			}
		}
		EVCALL = eq * totalpot - (1 - eq) * (call);

		stringstream stream;
		stream << "TOTALPOT= " << totalpot << endl;
		stream << "CALL= " << call << endl;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
		if (EVRAISE == -100000 && EVCALL < 0) return Action('n', 0);
	}

	if (game.getStreet() == 3)
	{
		logger.logExp("RIVER CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);

		int aktBetSize = normalizeBetSize(game.getStreet() + 1, game.getBiggestBet() * game.getBblind(), game.getPotcommon() * game.getBblind(), game.getBblind());

		for (int i = aktBetSize + 1; i < river.PLAYER_BET_SIZE_NUM && i < aktBetSize + 4; ++i)
		{
			double betsize = getBetsize(2, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			double maxraise = getMaxRaise(game) * game.getBblind();
			if (maxraise * 2 > betsize) betsize = maxraise * 2;

			logger.logExp("actualBetSize: ", i, LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(betsize, ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) break;
			
			vector<double> foldEquities = getFoldEquities(betsize, game, ranges, preflop, flop, turn, river);
			double evraise = calculateEVRaise(game, aktRanges, foldEquities, betsize);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = betsize;
			}
		}
		logger.logExp("START CALL EV CALCULATION\n", LOGGER_TYPE::BOT_LOGIC);
		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();
		double commonpot = game.getPotcommon() * game.getBblind();

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		EVCALL = eq * totalpot - (1 - eq) * (call);

		stringstream stream;
		stream << "TOTALPOT= " << totalpot << endl;
		stream << "CALL= " << call << endl;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
		if (EVRAISE == -100000 && EVCALL < 0) return Action('n', 0);
	}

	double totalpot = game.getTotalPot() * game.getBblind();

	logger.logExp("DECIDED ACTION\n", LOGGER_TYPE::BOT_LOGIC);
	if (EVRAISE > totalpot/10 || EVCALL > 0.04)
	{
		if (EVRAISE > EVCALL)
		{
			if (EVRAISE == -100000)
			{
				res.setAction('n', 0);
			}
			else
			{
				res.setAction('r', maxRaiseSize);
			}
		}
		else
		{
			if (EVRAISE == -100000)
			{
				logger.logExp("ACTION is hack\n", BOT_LOGIC);
				res.setAction('h', 0);
			}

			if (abs(game.getAmountToCall() - 0) < 0.001)
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