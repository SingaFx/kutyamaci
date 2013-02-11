#include "BwinPartyParser.h"

#include <boost/regex.hpp>
#include<boost/algorithm/string.hpp>
#include<boost/algorithm/string/replace.hpp>
#include "HandHistoryUtils.h"
#include "logger.h"

using namespace std;

BwinPartyParser::BwinPartyParser(int parserType, ifstream& fileHandle)
 : parserType(parserType)
 , fileHandle(fileHandle)
 , HAND_START("^\\*{5} Hand History for Game (.+) \\*{5}")
 , HAND_END("^Game #[[:digit:]]* starts\.$")
 , TABLE("^(Table Supersonic .*)$")
 , PLAYER("^Seat ([[:digit:]]*): (.*) \\( \\$(.*) \\)$")
 , BBLIND("^(.*) posts small blind \\[\\$(.*)")
 , CALL("^(.*) calls \\[\\$(.*)")
 , FOLD("^(.*) folds")
 , BET("^(.*) bets \\[\\$(.*)")
 , CHECK("^(.*) checks")
 , RAISE("^(.*) raises \\[\\$(.*)")
 , ALLIN("^(.*) is all-In  \\[\\$(.*)")
 , FLOP("^\\*{2} Dealing Flop \\*{2} \\[ (.)(.), (.)(.), (.)(.) \\]")
 , TURN("^\\*{2} Dealing Turn \\*{2} \\[ (.)(.) \\]")
 , RIVER("^\\*{2} Dealing River \\*{2} \\[ (.)(.) \\]")
 , SHOWDOWN("^(.*) shows \\[ (.)(.), (.)(.) \\]")
 , MUCK("^(.*) doesn't show \\[ (.)(.), (.)(.) \\]")
 , BUTTON("^Seat ([[:digit:]]*) is the button")
 , FILE("^Supersonic_[[:digit:]]*\.txt")
{
}

void BwinPartyParser::openFileForParsing(string fileName)
{
    fileHandle.open(fileName);
}

void BwinPartyParser::closeFileAfterParsing()
{
    fileHandle.close();
    //fileHandle.clear();
}

vector<HandHistory> BwinPartyParser::parse()
{
    vector<HandHistory> result;
	HandHistory actualhand;
	PlayerHistory actualPlayerHistory;
	string actualPlayer, playerName;

    string line;

    int round = 0;
	int buttonSeat = 0;

	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;

	boost::regex handstart(HAND_START);
	boost::regex handend(HAND_END);
	boost::regex table(TABLE);
	boost::regex player(PLAYER);
    boost::regex bblind(BBLIND);
	boost::regex call(CALL);
	boost::regex fold(FOLD);
	boost::regex bet(BET);
	boost::regex check(CHECK);
	boost::regex raise(RAISE);
    boost::regex allin(ALLIN);
	boost::regex flop(FLOP);
	boost::regex turn(TURN);
	boost::regex river(RIVER);
	boost::regex showdown(SHOWDOWN);
    boost::regex muck(MUCK);
	boost::regex button(BUTTON);

    bool foundFirstHandEnd = false;

    while (!fileHandle.eof() && !foundFirstHandEnd)
    {
		getline(fileHandle, line);
        if (regex_match(line, handend))
        {
            foundFirstHandEnd = true;
        }
    }

    bool isInHand = false;

	while (!fileHandle.eof())
	{
		getline(fileHandle, line);
		// Found a new hand
		if (regex_search(line, what, handstart, flags))
		{
			isInHand = true;
            actualhand.setId(string(what[1].first, what[1].second));
			actualhand.setFinalBetRound(0);
		}

		// Found end of hand
		else if (regex_match(line, handend))
		{
			isInHand = false;
            this->pushActualHandToParseResult(result, actualhand, buttonSeat);
            this->clearActualHandInfo(actualhand, round);
		}

		// Found the table
		else if (regex_search(line, what, table, flags))
		{
			actualhand.setTable(string(what[1].first, what[1].second));
		}

        // Found a big blind action
		else if (regex_search(line, what, bblind, flags))
		{
            actualhand.setBigBlind(atof(string(what[2].first,what[2].second).c_str()));
		}

		// Found a player
		else if (regex_search(line, what, player, flags))
		{
			actualPlayer = string(what[2].first, what[2].second);
            actualPlayer = boost::algorithm::erase_all_copy(actualPlayer, " " );
			if ( !HandHistoryUtils::playerExists(actualhand, actualPlayer) )
			{
				actualPlayerHistory.setPlayerName(actualPlayer);
				actualPlayerHistory.setBalance(atof(string(what[3].first,what[3].second).c_str()));
				actualPlayerHistory.setSeat(atoi(string(what[1].first,what[1].second).c_str()));
				actualPlayerHistory.getPreflopAction().clear();
				actualPlayerHistory.getFlopAction().clear();
				actualPlayerHistory.getTurnAction().clear();
				actualPlayerHistory.getRiverAction().clear();
				actualPlayerHistory.setHandKnown(false);
				actualhand.getPlayerHistories().push_back(actualPlayerHistory);
			}
		}
		// Found a call action
		else if (regex_search(line, what, call, flags))
		{
			Action tempAction;
            tempAction.setAction('c', atof(string(what[2].first,what[2].second).c_str()));
			playerName = string(what[1].first,what[1].second);
            playerName = boost::algorithm::erase_all_copy(playerName, " " );
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, playerName, round);
		}
		// Found a fold action
		else if (regex_search(line, what, fold, flags))
		{
			Action tempAction;
            tempAction.setAction('f', 0);
			playerName = string(what[1].first,what[1].second);
            playerName = boost::algorithm::erase_all_copy(playerName, " " );
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, playerName, round);
		}
		// Found a bet action
		else if (regex_search(line, what, bet, flags))
		{
			Action tempAction;
            tempAction.setAction('r', atof(string(what[2].first,what[2].second).c_str()));
			playerName = string(what[1].first,what[1].second);
            playerName = boost::algorithm::erase_all_copy(playerName, " " );
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, playerName, round);
		}
		// Found a check action
		else if (regex_search(line, what, check, flags))
		{
			Action tempAction;
            tempAction.setAction('x', 0);
			playerName = string(what[1].first,what[1].second);
            playerName = boost::algorithm::erase_all_copy(playerName, " " );
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, playerName, round);
		}
		// Found a raise action
		else if (regex_search(line, what, raise, flags))
		{
			Action tempAction;
            tempAction.setAction('r', atof(string(what[2].first,what[2].second).c_str()));
			playerName = string(what[1].first,what[1].second);
            playerName = boost::algorithm::erase_all_copy(playerName, " " );
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, playerName, round);
		}
        // Found an allin action
		else if (regex_search(line, what, allin, flags))
		{
			Action tempAction;
            tempAction.setAction('r', atof(string(what[2].first,what[2].second).c_str()));
			playerName = string(what[1].first,what[1].second);
            playerName = boost::algorithm::erase_all_copy(playerName, " " );
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, playerName, round);
		}
		// Found dealing flop
		else if (regex_search(line, what, flop, flags))
		{
			round = 1;
			actualhand.setFinalBetRound(1);
            char f1r, f1s, f2r, f2s, f3r, f3s;
			f1r = string(what[1].first,what[1].second).c_str()[0];
			f1s = string(what[2].first,what[2].second).c_str()[0];
			f2r = string(what[3].first,what[3].second).c_str()[0];
			f2s = string(what[4].first,what[4].second).c_str()[0];
			f3r = string(what[5].first,what[5].second).c_str()[0];
			f3s = string(what[6].first,what[6].second).c_str()[0];

            actualhand.setFlopCard1(Card(f1r, f1s));
            actualhand.setFlopCard2(Card(f2r, f2s));
            actualhand.setFlopCard3(Card(f3r, f3s));
		}
		// Found dealing turn
		else if (regex_search(line, what, turn, flags))
		{
			round = 2;
			actualhand.setFinalBetRound(2);
            char tr, ts;
            tr = string(what[1].first,what[1].second).c_str()[0];
			ts = string(what[2].first,what[2].second).c_str()[0];
            actualhand.setTurnCard(Card(tr, ts));
		}
		// Found dealing river
		else if (regex_search(line, what, river, flags))
		{
			round = 3;
            actualhand.setFinalBetRound(3);
            char rr, rs;
            rr = string(what[1].first,what[1].second).c_str()[0];
			rs = string(what[2].first,what[2].second).c_str()[0];
            actualhand.setRiverCard(Card(rr, rs));
		}
		else if (regex_search(line, what, button, flags))
		{
			buttonSeat = atoi(string(what[1].first,what[1].second).c_str());
		}
		// Found a player's hands at showdown
		else if (regex_search(line, what, showdown, flags))
		{
			Hand tempHand;
            char c1r, c1s, c2r, c2s;
            c1r = string(what[2].first,what[2].second).c_str()[0];
            c1s = string(what[3].first,what[3].second).c_str()[0];
            c2r = string(what[4].first,what[4].second).c_str()[0];;
            c2s = string(what[5].first,what[5].second).c_str()[0];
            tempHand.setHand(Card(c1r, c1s), Card(c2r, c2s));
            playerName = string(what[1].first,what[1].second);
            playerName = boost::algorithm::erase_all_copy(playerName, " " );
            HandHistoryUtils::setPlayersHand(actualhand, playerName, tempHand);
		}
        // Found a player's mucked hands at showdown
        else if (regex_search(line, what, muck, flags))
		{
			Hand tempHand;
            char c1r, c1s, c2r, c2s;
            c1r = string(what[2].first,what[2].second).c_str()[0];
            c1s = string(what[3].first,what[3].second).c_str()[0];
            c2r = string(what[4].first,what[4].second).c_str()[0];;
            c2s = string(what[5].first,what[5].second).c_str()[0];
            tempHand.setHand(Card(c1r, c1s), Card(c2r, c2s));
            playerName = string(what[1].first,what[1].second);
            playerName = boost::algorithm::erase_all_copy(playerName, " " );
            HandHistoryUtils::setPlayersHand(actualhand, playerName, tempHand);
		}
	}

    if (true == isInHand) {
        this->pushActualHandToParseResult(result, actualhand, buttonSeat);
        this->clearActualHandInfo(actualhand, round);
    }

	Logger& logger = Logger::getLogger(LOGGER_TYPE::HAND_HISTORY_PARSER);
    logger.logExp("The BWINPARSER has parsed the following number of hands: ", (int)result.size(), LOGGER_TYPE::HAND_HISTORY_PARSER);

    return result;
}

void BwinPartyParser::pushActualHandToParseResult(vector<HandHistory>& result, HandHistory& actualhand, int& buttonSeat)
{
    actualhand.setButtonSeat(buttonSeat);
	HandHistoryUtils::setPlayersPosition(actualhand, buttonSeat);
    if (false != HandHistoryUtils::isValidHandHistory(actualhand))
    {
        result.push_back(actualhand);
    }
}

void BwinPartyParser::clearActualHandInfo(HandHistory& actualhand, int& round)
{
    actualhand.getPlayerHistories().clear();
	round = 0;
}