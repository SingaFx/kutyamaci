#include "bayesDecision.h"
#include "eqcalculator.h"
#include "logger.h"
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
	totalRange = RangeUtils::mergeRange(range, totalRange, game.getBoard(), game.getHand());

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
	vector<PlayerRange> result;
	EqCalculator calc;
	PlayerRange myRange;
	myRange.range.insert(make_pair(hand, 1));

	result.push_back(myRange);

	for (int i = 0; i < ranges.size(); ++i)
	{
		result.push_back(ranges[i]);
	}


	return calc.calculate(result, board, 10000) / 100;
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
		if (nBetsize > 2 && nBetsize < 5)
		{
			double ratio = betsize / currentbet;

			if (ratio < 2.5)
			{
				FE = modifyValue(FE, -0.1);
			}
			
			if (ratio > 5)
			{
				FE = modifyValue(FE, 0.1);
			}
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
				FE = modifyValue(FE, 0.1);
			}
			else
			{
				FE = modifyValue(FE, 0.15);
			}
		}
		else
		{
			if (nBetsize == 0)
			{
				FE = modifyValue(FE, -0.2);
			}

			if (nBetsize == 1)
			{
				FE = modifyValue(FE, -0.1);
			}

			if (nBetsize == 2)
			{
			}

			if (nBetsize == 3)
			{
				FE = modifyValue(FE, 0.1);
			}

			if (nBetsize == 4)
			{
				FE = modifyValue(FE, 0.15);
			}

			if (nBetsize > 4)
			{
				FE = modifyValue(FE, 0.2);
			}

		}
	}

	return FE;
}

Action BayesDecision::makeDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(LOGGER_TYPE::BOT_LOGIC);

	logger.logExp("Hand: " + game.getHand().toString(), LOGGER_TYPE::BOT_LOGIC);

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
		for (int i = normalizeBetSize(1, game.getBiggestBet() * game.getBblind(), game.getPotcommon() * game.getBblind(), game.getBblind()) + 1; i < preflop.PREFLOP_PLAYER_BET_SIZE_NUM; ++i)
		{
			logger.logExp("actualBetSize: ", i, LOGGER_TYPE::BOT_LOGIC);

			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(getBetsize(1, i, game.getPotcommon() * game.getBblind(), game.getBblind()), ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) continue;
			
			double eq = calculateEQ(aktRanges, game.getBoard(), game.getHand());
			double fe = 0;

			double totalpot = game.getTotalPot() * game.getBblind();
			double betsize = getBetsize(1, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			double commonpot = game.getPotcommon() * game.getBblind();
			double heroCurrentBet = game.getHero().getBetsize() * game.getBblind();

			double evraise = fe * totalpot + (1 - fe) * (eq * (commonpot + (ranges.size() + 1) * betsize - betsize + heroCurrentBet) - (1 - eq) * (betsize - heroCurrentBet));

			stringstream stream;
			
			stream << "Totalpot: " << totalpot << " " << "BetSize: " << betsize << " " << "CommonPot: " << commonpot << " " << "heroCurrentBet: " << heroCurrentBet << endl;
			
			stream << "EQ= " << eq << endl;
			stream << "EVRAISE= " << evraise << endl;

			logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = getBetsize(1, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			}
		}

		if (EVRAISE == -100000)
		{
			logger.logExp("Cannot decide action EVRAISE == -10000", LOGGER_TYPE::BOT_LOGIC);
			return Action('n', 0);
		}


		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		EVCALL = eq * (totalpot + call) - (1 - eq) * (call);

		stringstream stream;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

	}
	
	if (game.getStreet() == 1)
	{
		logger.logExp("FLOP CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);
		for (int i = normalizeBetSize(game.getStreet() + 1, game.getBiggestBet() * game.getBblind(), game.getPotcommon() * game.getBblind(), game.getBblind()) + 1; i < flop.PLAYER_BET_SIZE_NUM; ++i)
		{
			logger.logExp("actualBetSize: ", i, LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(getBetsize(game.getStreet() + 1, i, game.getPotcommon() * game.getBblind(), game.getBblind()), ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) 
				{
					logger.logExp("Couldn't get opponent's calling+raising range.", LOGGER_TYPE::BOT_LOGIC);
					break;
				}
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) continue;
			
			double eq = calculateEQ(aktRanges, game.getBoard(), game.getHand());
			double fe = 1;

			for (int j = 0; j < ranges.size(); ++j)
			{
				CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
				double akt = flop.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 
					player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);

				if (akt < 0) 
				{
					fe = 0;
				}

				fe *= akt;
			}

			double totalpot = game.getTotalPot() * game.getBblind();
			double betsize = getBetsize(2, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			double commonpot = game.getPotcommon() * game.getBblind();
			double heroCurrentBet = game.getHero().getBetsize() * game.getBblind();

			double evraise = fe * totalpot + (1 - fe) * (eq * (commonpot + (ranges.size() + 1) * betsize - betsize + heroCurrentBet) - (1 - eq) * (betsize - heroCurrentBet));

			stringstream stream;
			
			stream << "Totalpot: " << totalpot << " " << "BetSize: " << betsize << " " << "CommonPot: " << commonpot << " " << "heroCurrentBet: " << heroCurrentBet << endl;
			
			stream << "FE= " << fe << endl;
			stream << "EQ= " << eq << endl;
			stream << "EVRAISE= " << evraise << endl;

			logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = getBetsize(game.getStreet() + 1, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			}
		}

		if (EVRAISE == -100000)
		{
			logger.logExp("Cannot decide action EVRAISE == -10000", LOGGER_TYPE::BOT_LOGIC);
			return Action('n', 0);
		}

		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		EVCALL = eq * (totalpot + call) - (1 - eq) * (call);

		stringstream stream;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}

	if (game.getStreet() == 2)
	{
		logger.logExp("TURN CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);
		for (int i = normalizeBetSize(game.getStreet() + 1, game.getBiggestBet() * game.getBblind(), game.getPotcommon() * game.getBblind(), game.getBblind()) + 1; i < turn.PLAYER_BET_SIZE_NUM; ++i)
		{
			logger.logExp("actualBetSize: ", i, LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(getBetsize(game.getStreet() + 1, i, game.getPotcommon() * game.getBblind(), game.getBblind()), ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) continue;
			
			double eq = calculateEQ(aktRanges, game.getBoard(), game.getHand());
			double fe = 1;

			for (int j = 0; j < ranges.size(); ++j)
			{
				CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
				double akt = turn.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 
					player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);

				if (akt < 0) 
				{
					fe = 0;
				}

				fe *= akt;
			}

			double totalpot = game.getTotalPot() * game.getBblind();
			double betsize = getBetsize(2, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			double commonpot = game.getPotcommon() * game.getBblind();
			double heroCurrentBet = game.getHero().getBetsize() * game.getBblind();

			double evraise = fe * totalpot + (1 - fe) * (eq * (commonpot + (ranges.size() + 1) * betsize - betsize + heroCurrentBet) - (1 - eq) * (betsize - heroCurrentBet));

			stringstream stream;
			
			stream << "Totalpot: " << totalpot << " " << "BetSize: " << betsize << " " << "CommonPot: " << commonpot << " " << "heroCurrentBet: " << heroCurrentBet << endl;
		
			stream << "FE= " << fe << endl;
			stream << "EQ= " << eq << endl;
			stream << "EVRAISE= " << evraise << endl;

			logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = getBetsize(game.getStreet() + 1, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			}
		}

		if (EVRAISE == -100000) return Action('n', 0);

		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		EVCALL = eq * (totalpot + call) - (1 - eq) * (call);

		stringstream stream;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}

	if (game.getStreet() == 3)
	{
		logger.logExp("RIVER CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);
		for (int i = normalizeBetSize(game.getStreet() + 1, game.getBiggestBet() * game.getBblind(), game.getPotcommon() * game.getBblind(), game.getBblind()) + 1; i < river.PLAYER_BET_SIZE_NUM; ++i)
		{
			logger.logExp("actualBetSize: ", i, LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(getBetsize(game.getStreet() + 1, i, game.getPotcommon() * game.getBblind(), game.getBblind()), ranges[j], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) continue;
			
			double eq = calculateEQ(aktRanges, game.getBoard(), game.getHand());
			double fe = 1;

			for (int j = 0; j < ranges.size(); ++j)
			{
				CurrentPlayerInfo player = game.getPlayerbyId(ranges[j].getId());
				double akt = river.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 
					player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);

				if (akt < 0) 
				{
					fe = 0;
				}

				fe *= akt;
			}

			double totalpot = game.getTotalPot() * game.getBblind();
			double betsize = getBetsize(2, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			double commonpot = game.getPotcommon() * game.getBblind();
			double heroCurrentBet = game.getHero().getBetsize() * game.getBblind();

			double evraise = fe * totalpot + (1 - fe) * (eq * (commonpot + (ranges.size() + 1) * betsize - betsize + heroCurrentBet) - (1 - eq) * (betsize - heroCurrentBet));

			stringstream stream;
			
			stream << "Totalpot: " << totalpot << " " << "BetSize: " << betsize << " " << "CommonPot: " << commonpot << " " << "heroCurrentBet: " << heroCurrentBet << endl;
			
			stream << "FE= " << fe << endl;
			stream << "EQ= " << eq << endl;
			stream << "EVRAISE= " << evraise << endl;

			logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = getBetsize(game.getStreet() + 1, i, game.getPotcommon() * game.getBblind(), game.getBblind());
			}
		}

		if (EVRAISE == -100000) return Action('n', 0);
		
		double totalpot = game.getTotalPot() * game.getBblind();
		double call = game.getAmountToCall() * game.getBblind();

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		EVCALL = eq * (totalpot + call) - (1 - eq) * (call);

		stringstream stream;
		stream << "EQ= " << eq << endl;
		stream << "EVCALL= " << EVCALL << endl;

		logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);
	}

	if (EVRAISE > 0 || EVCALL > 0)
	{
		if (EVRAISE > EVCALL)
		{
			res.setAction('r', maxRaiseSize);
		}
		else
		{
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