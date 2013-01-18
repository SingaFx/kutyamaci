#include "plusEVBotLogic.h"
#include "rangeFunctions.h"

PlusEVBotLogic::PlusEVBotLogic()
{
	database = new Database("127.0.0.1", "root", "root", "kutya");
	preflop.read("preflopBayes");
	flop.read("flopBayes");
	turn.read("turnBayes");
	river.read("riverBayes");
}

PlayerRange& PlusEVBotLogic::calculateRange(string name, CurrentGameInfo& gameInfo, PlayerRange& oldPlayerRange)
{
	extendGameInfo(gameInfo);

	PlayerRange res;

	if (gameInfo.getStreet() == 0)
	{
		CurrentPlayerInfo player = gameInfo.getPlayerByName(name);
		PlayerRange actual = preflop.getRange(player.getVPIP(), player.getPFR(), player.getStacksize(), player.getPoz(), player.getLine(), player.getBetsize(), gameInfo.getBblind(), 0, patternsNeeded);

		vector<Card> cards;
		res = RangeUtils::mergeRange(oldPlayerRange, actual, cards);
	}

	if (gameInfo.getStreet() == 1)
	{
		CurrentPlayerInfo player = gameInfo.getPlayerByName(name);
		
		PlayerRange actual = flop.getRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize(), player.getLine(), player.getBetsize(), gameInfo.getBblind(), 
			gameInfo.getPotcommon(), gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);

		res = RangeUtils::mergeRange(oldPlayerRange, actual, gameInfo.getBoard());
	}

	if (gameInfo.getStreet() == 2)
	{
		CurrentPlayerInfo player = gameInfo.getPlayerByName(name);
		
		PlayerRange actual = turn.getRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize(), player.getLine(), player.getBetsize(), gameInfo.getBblind(), 
											gameInfo.getPotcommon(), gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);

		res = RangeUtils::mergeRange(oldPlayerRange, actual, gameInfo.getBoard());
	}

	if (gameInfo.getStreet() == 3)
	{
		CurrentPlayerInfo player = gameInfo.getPlayerByName(name);
		
		PlayerRange actual = river.getRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize(), player.getLine(), player.getBetsize(), gameInfo.getBblind(), 
											gameInfo.getPotcommon(), gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);

		res = RangeUtils::mergeRange(oldPlayerRange, actual, gameInfo.getBoard());
	}


	return res;
}

Action PlusEVBotLogic::makeDecision(CurrentGameInfo& gameInfo, vector<PlayerRange>& ranges)
{
	Action res;

	extendGameInfo(gameInfo);

	res = BayesDecision::makeDecision(gameInfo, ranges);
	if (res.getType() == 5)
	{
		res = SimpleDecision::makeDecision(gameInfo);
	}

	return res;
}

CurrentPlayerInfo& PlusEVBotLogic::setStandardPlayerType(CurrentPlayerInfo& playerInfo, double bblind)
{
	CurrentPlayerInfo result = playerInfo;

	if (playerInfo.getStacksize() < 100)
	{
		result.setVPIP(40);
		result.setPFR(10);
		result.setAF(1);
	}
	else
	{
		result.setVPIP(20);
		result.setPFR(15);
		result.setAF(3.5);
	}

	return result;
}
void PlusEVBotLogic::extendGameInfo(CurrentGameInfo& gameInfo)
{
	for (int i = 0; i < gameInfo.getOpponentsInfo().size(); ++i)
	{
		string name = gameInfo.getOpponentsInfo()[i].getName();

		if (database->isUser(name))
		{
			gameInfo.getOpponentsInfo()[i].setHandnr(database->getHandnr(name));
			if (gameInfo.getOpponentsInfo()[i].getHandnr() < 5)
			{
				gameInfo.getOpponentsInfo()[i] = setStandardPlayerType(gameInfo.getOpponentsInfo()[i], gameInfo.getBblind());
			}
			else
			{
				gameInfo.getOpponentsInfo()[i].setVPIP(database->getVPIP(name));
				gameInfo.getOpponentsInfo()[i].setPFR(database->getPFR(name));
				gameInfo.getOpponentsInfo()[i].setAF(database->getAF(name));
			}
		}
		else
		{
			gameInfo.getOpponentsInfo()[i] = setStandardPlayerType(gameInfo.getOpponentsInfo()[i], gameInfo.getBblind());
		}
	}
}