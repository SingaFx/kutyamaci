#include "plusEVBotLogic.h"
#include "rangeFunctions.h"
#include "logger.h"

PlusEVBotLogic::PlusEVBotLogic()
{
	database = new Database("127.0.0.1", "root", "root", "kutya");
	preflop.read("preflopBayes");
	flop.read("flopBayes");
	turn.read("turnBayes");
	river.read("riverBayes");
}
PlusEVBotLogic::~PlusEVBotLogic()
{

}

PlayerRange PlusEVBotLogic::calculateRange(int id, CurrentGameInfo& gameInfo, PlayerRange& oldPlayerRange)
{
	//bb checks
	if (gameInfo.getStreet() == 0)
	{
		CurrentPlayerInfo player = gameInfo.getPlayerbyId(id);
		if (player.getPoz() == 2 && player.getLine() == 2)
		{
			return oldPlayerRange;
		}
	}
	

	//validation
	if (!oldPlayerRange.getValid()) return oldPlayerRange;

	extendGameInfo(gameInfo);

	Logger& logger = Logger::getLogger(BOT_LOGIC);

	PlayerRange res;
	double bblind = gameInfo.getBblind();

	if (gameInfo.getStreet() == 0)
	{
		CurrentPlayerInfo player = gameInfo.getPlayerbyId(id);
		
		res = preflop.getRange(player.getVPIP(), player.getPFR(), player.getStacksize() * bblind, player.getPoz(), player.getLine(), player.getBetsize() * bblind, gameInfo.getBblind(), 0, patternsNeeded);

		
		ostringstream os;
		os << "Preflop range query; Input : VPIP : " << player.getVPIP() << " PFR : " << player.getPFR() << " StackSize : " << player.getStacksize() << " poz: " << player.getPoz() << " line : " << player.getLine() << " betsize : " <<
			player.getBetsize() * bblind << " bblind : " << gameInfo.getBblind() << endl;

		
		/*os << "Old range : " << oldPlayerRange.totalPercentage() << "; New range : " << res.totalPercentage();
		os << endl << endl << "------------------------------- OLD RANGE ---------------------------------" << oldPlayerRange.toString() << "--------------------------------------------------------------------" << endl << endl;
		os << endl << endl << "------------------------------- NEW RANGE ---------------------------------" << res.toString() << "--------------------------------------------------------------------" << endl << endl;
		*/
		logger.logExp(os.str(), BOT_LOGIC);
	}

	if (gameInfo.getStreet() == 1)
	{
		CurrentPlayerInfo player = gameInfo.getPlayerbyId(id);
		
		PlayerRange actual = flop.getRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * bblind, player.getLine(), player.getBetsize() * bblind, gameInfo.getBblind(), 
			gameInfo.getPotcommon() * bblind, gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);

		res = RangeUtils::mergeRange(oldPlayerRange, actual, gameInfo.getBoard(), gameInfo.getHand());

		ostringstream os;
		os << "Flop range query; Input : VPIP : " << player.getVPIP() << " PFR : " << player.getPFR() << "AF : " << player.getAF() << " StackSize : " << player.getStacksize() << " line : " << player.getLine() << " betsize : " <<
			player.getBetsize() << " bblind : " << gameInfo.getBblind() << " Potcommon : " << gameInfo.getPotcommon() << endl;

		/*os << "Old range : " << oldPlayerRange.totalPercentage() << "; New range : " << res.totalPercentage();
		os << endl << endl << "------------------------------- OLD RANGE ---------------------------------" << endl << oldPlayerRange.toString() << "--------------------------------------------------------------------" << endl << endl;
		os << endl << endl << "------------------------------- NEW RANGE ---------------------------------" << endl << actual.toString() << "--------------------------------------------------------------------" << endl << endl;
		os << endl << endl << "------------------------------- MERGED RANGE ------------------------------" << endl << res.toString() << "--------------------------------------------------------------------" << endl << endl;
		*/
		logger.logExp(os.str(), BOT_LOGIC);
	}

	if (gameInfo.getStreet() == 2)
	{
		CurrentPlayerInfo player = gameInfo.getPlayerbyId(id);
		
		PlayerRange actual = turn.getRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize() * bblind, player.getLine(), player.getBetsize() * bblind, gameInfo.getBblind(), 
											gameInfo.getPotcommon() * bblind, gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);

		res = RangeUtils::mergeRange(oldPlayerRange, actual, gameInfo.getBoard(), gameInfo.getHand());

		ostringstream os;
		os << "Turn range query; Input : VPIP : " << player.getVPIP() << " PFR : " << player.getPFR() << "AF : " << player.getAF() << " StackSize : " << player.getStacksize() << " line : " << player.getLine() << " betsize : " <<
			player.getBetsize() << " bblind : " << gameInfo.getBblind() << " Potcommon : " << gameInfo.getPotcommon() << endl;

		os << "Old range : " << oldPlayerRange.totalPercentage() << "; New range : " << res.totalPercentage();
		os << endl << endl << "------------------------------- OLD RANGE ---------------------------------" << endl << oldPlayerRange.toString() << "--------------------------------------------------------------------" << endl << endl;
		os << endl << endl << "------------------------------- NEW RANGE ---------------------------------" << endl << actual.toString() << "--------------------------------------------------------------------" << endl << endl;
		os << endl << endl << "------------------------------- MERGED RANGE ------------------------------" << endl << res.toString() << "--------------------------------------------------------------------" << endl << endl;
		
		logger.logExp(os.str(), BOT_LOGIC);
	}

	if (gameInfo.getStreet() == 3)
	{
		CurrentPlayerInfo player = gameInfo.getPlayerbyId(id);
		
		PlayerRange actual = river.getRange(player.getVPIP(), player.getPFR(), player.getAF(), player.getStacksize(), player.getLine(), player.getBetsize(), gameInfo.getBblind(), 
											gameInfo.getPotcommon(), gameInfo.getBoard(), gameInfo.getHand(), patternsNeeded);

		res = RangeUtils::mergeRange(oldPlayerRange, actual, gameInfo.getBoard(), gameInfo.getHand());

		ostringstream os;
		os << "River range query; Input : VPIP : " << player.getVPIP() << " PFR : " << player.getPFR() << "AF : " << player.getAF() << " StackSize : " << player.getStacksize() << " line : " << player.getLine() << " betsize : " <<
			player.getBetsize() << " bblind : " << gameInfo.getBblind() << " Potcommon : " << gameInfo.getPotcommon() << endl;

		os << "Old range : " << oldPlayerRange.totalPercentage() << "; New range : " << res.totalPercentage();
		os << endl << endl << "------------------------------- OLD RANGE ---------------------------------" << endl << oldPlayerRange.toString() << "--------------------------------------------------------------------" << endl << endl;
		os << endl << endl << "------------------------------- NEW RANGE ---------------------------------" << endl << actual.toString() << "--------------------------------------------------------------------" << endl << endl;
		os << endl << endl << "------------------------------- MERGED RANGE ------------------------------" << endl << res.toString() << "--------------------------------------------------------------------" << endl << endl;
		
		logger.logExp(os.str(), BOT_LOGIC);
	}

	if (res.range.size() == 0)
	{
		CurrentPlayerInfo player = gameInfo.getPlayerbyId(id);
		if (player.getLine() == 2)
		{
			logger.logExp("Not enough patterns for check!!!", BOT_LOGIC);
			return oldPlayerRange;
		}

		oldPlayerRange.setValid(false);
		logger.logExp("ERROR in range!!!", BOT_LOGIC);

		return oldPlayerRange;
	}

	res.setValid(true);
	return res;
}

Action PlusEVBotLogic::makeDecision(CurrentGameInfo& gameInfo, vector<PlayerRange>& ranges)
{
	Logger& logger = logger.getLogger(BOT_LOGIC);
	logger.logExp("game street: ", gameInfo.getStreet(), LOGGER_TYPE::BOT_LOGIC);
	if (gameInfo.getStreet() == 0 && preflopOpenRaise(gameInfo))
	{
		logger.logExp("PREFLOP game street: ", gameInfo.getStreet(), LOGGER_TYPE::BOT_LOGIC);
		return Action('n', 0);
	}

	Action res;

	extendGameInfo(gameInfo);

	res = BayesDecision::makeDecision(gameInfo, ranges, preflop, flop, turn, river);
	if (res.getType() == 'n')
	{
		//res = SimpleDecision::makeDecision(gameInfo);
	}

	return res;
}

CurrentPlayerInfo PlusEVBotLogic::setStandardPlayerType(CurrentPlayerInfo& playerInfo, double bblind)
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

bool PlusEVBotLogic::preflopOpenRaise(CurrentGameInfo& gameInfo)
{
	for (int i = 0; i < gameInfo.getOpponentsInfo().size(); ++i)
	{
		CurrentPlayerInfo player = gameInfo.getOpponentsInfo()[i];
		if (player.getBetsize() > 1) return false; 
	}

	return true;
}