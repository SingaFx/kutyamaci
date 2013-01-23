#include "OnGameParser.h"

#include <boost/regex.hpp>
#include "HandHistoryUtils.h"

vector<HandHistory> OnGameParser::parse()
{
	//cout << "Begin parsing file: " << filename << endl;
	ifstream file;
	/*file.open(filename);*/

	vector<HandHistory> result;
	HandHistory actualhand;
	PlayerHistory actualPlayerHistory;
	string actualPlayer;
	string line;
	int round = 0;
	int buttonSeat = 0;
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;

	boost::regex handstart("^\\*\\*\\*\\*\\* History for hand (.+) \\*\\*\\*\\*\\*");
	boost::regex handend("^\\*\\*\\*\\*\\* End of hand.*");
	boost::regex table("^Table: (.+) \\[.*");
	boost::regex player("^Seat ([[:digit:]]*): (.*) \\(\\$(.*)\\)$");
	boost::regex call("^(.*) calls \\$(.*)");
	boost::regex fold("^(.*) folds");
	boost::regex bet("^(.*) bets \\$(.*)");
	boost::regex check("^(.*) checks");
	boost::regex raise("^(.*) raises .* to \\$(.*)");
	boost::regex flop("^--- Dealing flop \\[(.)(.), (.)(.), (.)(.)\\]");
	boost::regex turn("^--- Dealing turn \\[(.)(.)\\]");
	boost::regex river("^--- Dealing river \\[(.)(.)\\]");
	boost::regex showdown("^Seat [[:digit:]]*: (.*) \\(\\$.*\\), net: .*\\[(.)(.), (.)(.)\\].*");
	boost::regex button("^Button: seat ([[:digit:]]*)");

	while(!file.eof())
	{
		getline(file,line);

		// Found a new hand
		if (regex_search(line, what, handstart, flags))
		{
			actualhand.setId(string(what[1].first,what[1].second));
			actualhand.setFinalBetRound(0);
		}

		// Found end of hand
		else if (regex_match(line, handend))
		{
			actualhand.setButtonSeat(buttonSeat);
			HandHistoryUtils::setPlayersPosition(actualhand, buttonSeat);
			result.push_back(actualhand);
			actualhand.getPlayerHistories().clear();
			round = 0;
		}
		// Found the table
		else if (regex_search(line, what, table, flags))
		{
			actualhand.setTable(string(what[1].first,what[1].second));
		}

		// Found a player
		else if (regex_search(line, what, player, flags))
		{
			actualPlayer = string(what[2].first,what[2].second);
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
			string player_name = string(what[1].first,what[1].second);
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, player_name, round);
		}
		// Found a fold action
		else if (regex_search(line, what, fold, flags))
		{
			Action tempAction;
            tempAction.setAction(0, 'f');
			string player_name = string(what[1].first,what[1].second);
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, player_name, round);
		}
		// Found a bet action
		else if (regex_search(line, what, bet, flags))
		{
			Action tempAction;
            tempAction.setAction('r', atof(string(what[2].first,what[2].second).c_str()));
			string player_name = string(what[1].first,what[1].second);
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, player_name, round);
		}
		// Found a check action
		else if (regex_search(line, what, check, flags))
		{
			Action tempAction;
            tempAction.setAction(0, 'x');
			string player_name = string(what[1].first,what[1].second);
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, player_name, round);
		}
		// Found a raise action
		else if (regex_search(line, what, raise, flags))
		{
			Action tempAction;
            tempAction.setAction('r', atof(string(what[2].first,what[2].second).c_str()));
			string player_name = string(what[1].first,what[1].second);
			HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, player_name, round);
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
		if (regex_search(line, what, showdown, flags))
		{
			Hand tempHand;
            char c1r, c1s, c2r, c2s;
            c1r = string(what[2].first,what[2].second).c_str()[0];
            c1s = string(what[3].first,what[3].second).c_str()[0];
            c2r = string(what[4].first,what[4].second).c_str()[0];;
            c2s = string(what[5].first,what[5].second).c_str()[0];
            tempHand.setHand(Card(c1r, c1s), Card(c2r, c2s));
            HandHistoryUtils::setPlayersHand(actualhand, string(what[1].first,what[1].second), tempHand);
		}
	}

	file.close();
	cout << "End parsing file" << endl;
	return result;
}