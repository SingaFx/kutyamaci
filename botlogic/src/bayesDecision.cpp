#include "bayesDecision.h"
#include "eqcalculator.h"
#include "logger.h"
#include <sstream>

PlayerRange& BayesDecision::getCallRaiseRange(double betsize, PlayerRange& range, CurrentGameInfo& game, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	PlayerRange res;
	PlayerRange totalRange;

	CurrentPlayerInfo player = game.getPlayerByName(range.getName());

	if (game.getStreet() == 0)
	{
		totalRange = preflop.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getStacksize() * game.getBblind(), player.getPoz(), 0, betsize * game.getBblind(), game.getBblind(), 0, patternsNeeded);
	}

	if (game.getStreet() == 1)
	{
		totalRange = flop.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 0, betsize * game.getBblind(), game.getBblind(), 
			game.getPotcommon() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
	}

	if (game.getStreet() == 2)
	{
		totalRange = turn.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 0, betsize * game.getBblind(), game.getBblind(), 
			game.getPotcommon() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
	}

	if (game.getStreet() == 3)
	{
		totalRange = river.getCallRaiseRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 0, betsize * game.getBblind(), game.getBblind(), 
			game.getPotcommon() * game.getBblind(), game.getBoard(), game.getHand(), patternsNeeded);
	}

	if (totalRange.range.size() == 0) return res;
	totalRange = RangeUtils::mergeRange(range, totalRange, game.getBoard(), game.getHand());

	return res;
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
	return calc.calculate(result, board, 25000);
}

char BayesDecision::calculateDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	char res = 'n';

	return res;
}

Action BayesDecision::makeDecision(CurrentGameInfo& game, vector<PlayerRange>& rangesFirst, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	Logger& logger = Logger::getLogger(LOGGER_TYPE::BOT_LOGIC);

	logger.logExp("Hand: " + game.getHand().toString(), LOGGER_TYPE::BOT_LOGIC);

	vector<PlayerRange> ranges;

	for (int i = 0; i < rangesFirst.size(); ++i)
	{
		if (!rangesFirst[i].getPreflopNotPlaying())
		{
			ranges.push_back(rangesFirst[i]);
		}
	}

	if (ranges.size() == 0)
	{
		logger.logExp("Cannot decide action", LOGGER_TYPE::BOT_LOGIC);
		return Action('n', 0);
	}

	for (int i = 0; i < ranges.size(); ++i)
	{
		if (!ranges[i].getValid())
		{
			logger.logExp("Cannot decide action", LOGGER_TYPE::BOT_LOGIC);
			return Action('n', 0);
		}
	}

	Action res;

	//FE = FE1 * FE2 * ...
	//EVRAISE = FE * totalpot + (1 - FE) * (EQ * (totalpot + (nopponents + 1) * betsize) - (1 - EQ) * (betsize))
	//--> EQ vs calling + raiserange
	//--> FE: novelni, ha pozicioban van!
	//??--> minEQ ahhoz h beteljen (flop 20%, turn+ 30%)
	//EVCALL = EQ * (totalpot + call) - (1 - EQ) * call
	
	//--> LINE EV?

	double EVRAISE = -100000;
	double maxRaiseSize = 0;

	double EVCALL = 0;

	if (game.getStreet() == 0)
	{
		logger.logExp("PREFLOP CALCULATION STARTED", LOGGER_TYPE::BOT_LOGIC);
		for (int i = normalizeBetSize(1, game.getBiggestBet() * game.getBblind(), game.getPotcommon() * game.getBblind(), game.getBblind()) + 1; i < preflop.PREFLOP_PLAYER_BET_SIZE_NUM; ++i)
		{
			logger.logExp("actualBetSize: " + i, LOGGER_TYPE::BOT_LOGIC);

			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(getBetsize(1, i, game.getPotcommon()), ranges[i], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) continue;
			
			double eq = calculateEQ(aktRanges, game.getBoard(), game.getHand());
			double fe = 0;

			double evraise = fe * game.getTotalPot() + (1 - fe) * (eq * (game.getTotalPot() + (ranges.size() + 1) * getBetsize(1, i, game.getPotcommon()) - (1 - eq) * (getBetsize(1, i, game.getPotcommon()))));

			stringstream stream;
			stream << "EQ= " << eq << endl;
			stream << "EVRAISE= " << evraise << endl;

			logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = getBetsize(1, i, game.getPotcommon());
			}
		}

		if (EVRAISE == -100000) return Action('n', 0);

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		EVCALL = eq * (game.getTotalPot() + game.getAmountToCall()) - (1 - eq) * (game.getAmountToCall());

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
			logger.logExp("actualBetSize: " + i, LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(getBetsize(game.getStreet() + 1, i, game.getPotcommon()), ranges[i], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) continue;
			
			double eq = calculateEQ(aktRanges, game.getBoard(), game.getHand());
			double fe = 1;

			for (int j = 0; j < ranges.size(); ++j)
			{
				CurrentPlayerInfo player = game.getPlayerByName(ranges[i].getName());
				double akt = flop.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 
					player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);

				if (akt < 0) 
				{
					fe = 0;
				}

				fe *= akt;
			}

			double evraise = fe * game.getTotalPot() + (1 - fe) * (eq * (game.getTotalPot() + (ranges.size() + 1) * getBetsize(game.getStreet() + 1, i, game.getPotcommon()) - (1 - eq) * (getBetsize(game.getStreet() + 1, i, game.getPotcommon()))));

			stringstream stream;
			stream << "FE= " << fe << endl;
			stream << "EQ= " << eq << endl;
			stream << "EVRAISE= " << evraise << endl;

			logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = getBetsize(game.getStreet() + 1, i, game.getPotcommon());
			}
		}

		if (EVRAISE == -100000) return Action('n', 0);

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		EVCALL = eq * (game.getTotalPot() + game.getAmountToCall()) - (1 - eq) * (game.getAmountToCall());

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
			logger.logExp("actualBetSize: " + i, LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(getBetsize(game.getStreet() + 1, i, game.getPotcommon()), ranges[i], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) continue;
			
			double eq = calculateEQ(aktRanges, game.getBoard(), game.getHand());
			double fe = 1;

			for (int j = 0; j < ranges.size(); ++j)
			{
				CurrentPlayerInfo player = game.getPlayerByName(ranges[i].getName());
				double akt = turn.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 
					player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);

				if (akt < 0) 
				{
					fe = 0;
				}

				fe *= akt;
			}

			double evraise = fe * game.getTotalPot() + (1 - fe) * (eq * (game.getTotalPot() + (ranges.size() + 1) * getBetsize(game.getStreet() + 1, i, game.getPotcommon()) - (1 - eq) * (getBetsize(game.getStreet() + 1, i, game.getPotcommon()))));

			stringstream stream;
			stream << "FE= " << fe << endl;
			stream << "EQ= " << eq << endl;
			stream << "EVRAISE= " << evraise << endl;

			logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = getBetsize(game.getStreet() + 1, i, game.getPotcommon());
			}
		}

		if (EVRAISE == -100000) return Action('n', 0);

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		EVCALL = eq * (game.getTotalPot() + game.getAmountToCall()) - (1 - eq) * (game.getAmountToCall());

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
			logger.logExp("actualBetSize: " + i, LOGGER_TYPE::BOT_LOGIC);
			vector<PlayerRange> aktRanges;
			for (int j = 0; j < ranges.size(); ++j)
			{
				PlayerRange akt = getCallRaiseRange(getBetsize(game.getStreet() + 1, i, game.getPotcommon()), ranges[i], game, preflop, flop, turn, river);
				if (akt.range.size() == 0) break;
				aktRanges.push_back(akt);
			}
			
			if (aktRanges.size() != ranges.size()) continue;
			
			double eq = calculateEQ(aktRanges, game.getBoard(), game.getHand());
			double fe = 1;

			for (int j = 0; j < ranges.size(); ++j)
			{
				CurrentPlayerInfo player = game.getPlayerByName(ranges[i].getName());
				double akt = river.getProbabilityFE(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * game.getBblind(), 
					player.getLine(), player.getBetsize() * game.getBblind(), game.getBblind(), game.getPotcommon() * game.getBblind(), patternsNeeded);

				if (akt < 0) 
				{
					fe = 0;
				}

				fe *= akt;
			}

			double evraise = fe * game.getTotalPot() + (1 - fe) * (eq * (game.getTotalPot() + (ranges.size() + 1) * getBetsize(game.getStreet() + 1, i, game.getPotcommon()) - (1 - eq) * (getBetsize(game.getStreet() + 1, i, game.getPotcommon()))));

			stringstream stream;
			stream << "FE= " << fe << endl;
			stream << "EQ= " << eq << endl;
			stream << "EVRAISE= " << evraise << endl;

			logger.logExp(stream.str(), LOGGER_TYPE::BOT_LOGIC);

			if (evraise > EVRAISE)
			{
				EVRAISE = evraise;
				maxRaiseSize = getBetsize(game.getStreet() + 1, i, game.getPotcommon());
			}
		}

		if (EVRAISE == -100000) return Action('n', 0);

		double eq = calculateEQ(ranges, game.getBoard(), game.getHand());
		EVCALL = eq * (game.getTotalPot() + game.getAmountToCall()) - (1 - eq) * (game.getAmountToCall());

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
			if (game.getAmountToCall() == 0)
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