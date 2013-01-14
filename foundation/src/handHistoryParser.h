#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>

using namespace std;

class Card
{
public:
	char rank,suit;

	Card()
	{
		rank = suit = 0;
	}

	Card(char rank, char suit)
	{
		this->rank = rank;
		this->suit = suit;
	}

	bool operator==(const Card& c) const
	{
		return c.rank == rank && c.suit == suit; 
	}
};

class Hand
{
public:
	Card card1,card2;

	Hand()
	{
	}

	Hand(Card card1, Card card2)
	{
		this->card1 = card1;
		this->card2 = card2;
	}

	bool operator<(const Hand& h) const
	{
		if (h.card1.rank == card1.rank)
		{
			if (h.card2.rank == card2.rank)
			{
				if (h.card1.suit == card1.suit)
				{
					return h.card2.suit < card2.suit;
				}
				return h.card1.suit < card1.suit;
			}
			return h.card2.rank < card2.rank;
		}
		return h.card1.rank < card1.rank;
	}

	bool operator==(const Hand& h) const
	{
		return h.card1 == card1 && h.card2 == card2;
	}
};

class Player
{
public:
	string name;
};

class Action
{
public:
	char type;
	double size;
};

class PlayerHistory
{
public:
	Player player;
	int seat;
	int position;
	double balance;
	bool handKnown;
	Hand hand;
	vector<Action> preflopAction;
	vector<Action> flopAction;
	vector<Action> turnAction;
	vector<Action> riverAction;
};

class HandHistory
{
public:
	string id;
	string table;
	vector<PlayerHistory> players;
	Hand flopHand1, flopHand2, flopHand3, turnHand, riverHand;
	int finalBetRound;
	int buttonSeat;
};

class HandHistoryUtils
{
public:
	static bool playerExists(HandHistory &history, string playername)
	{
		vector<PlayerHistory>::iterator it;
		for (it = history.players.begin(); it != history.players.end(); ++it)
		{
			if ((*it).player.name == playername)
			{
				return true;
			}
		}

		return false;
	}

	static void addActiontoPlayer(HandHistory &history, Action &action, string playername, int round)
	{
		vector<PlayerHistory>::iterator it;
		for (it = history.players.begin(); it != history.players.end(); ++it)
		{
			if ((*it).player.name == playername)
			{
				switch(round)
				{
				case 0:
					(*it).preflopAction.push_back(action);
					break;
				case 1:
					(*it).flopAction.push_back(action);
					break;
				case 2:
					(*it).turnAction.push_back(action);
					break;
				case 3:
					(*it).riverAction.push_back(action);
					break;
				}
				break;
			}
		}
	}

	static void setPlayersHand(HandHistory &history, string playername, Hand &hand)
	{
		vector<PlayerHistory>::iterator it;
		for (it = history.players.begin(); it != history.players.end(); ++it)
		{
			if ((*it).player.name == playername)
			{
				(*it).handKnown = true;
				(*it).hand = hand;
				break;
			}
		}
	}

	static void setPlayersPosition(HandHistory &history, int buttonSeat)
	{
		int nr = history.players.size();
		int buttonnr = 0;
		
		for (int i = 0; i < nr; ++i)
		{
			if (history.players[i].seat == buttonSeat)
			{
				buttonnr = i;
				history.players[i].position = 0;
			}
		}

		for (int i = buttonnr+1; i < nr; ++i)
		{
			switch (i-buttonnr)
			{
			case 1:
			case 2:
				history.players[i].position = i-buttonnr;
				break;
			default:
				history.players[i].position = i-nr-buttonnr;
				break;
			}
		}

		for (int i = 0; i < buttonnr; ++i)
		{
			switch(i-buttonnr)
			{
			case -5:
				history.players[i].position = 1;
				break;
			case -4:
				history.players[i].position = 2;
				break;
			default:
				history.players[i].position = i - buttonnr;
				break;
			}
		}
	}

	static void detailedExportToFile(vector<HandHistory> &history, string filename)
	{
		int nrofhands = 0;
		ofstream file;
		file.open(filename, fstream::app);

		file << "Number of hands : " << history.size() << endl;

		vector<HandHistory>::iterator it;
		vector<PlayerHistory>::iterator itt;
		vector<Action>::iterator itAction;
		for (it=history.begin();it!=history.end();++it)
		{
			file << "Hand " << (*it).id << endl;
			file << "Table : " << (*it).table << endl;
			if ((*it).finalBetRound >= 1)
			{
				file << "Flop : " << (*it).flopHand1.card1.rank << (*it).flopHand1.card1.suit << " ";
				file << (*it).flopHand2.card1.rank << (*it).flopHand2.card1.suit << " ";
				file << (*it).flopHand3.card1.rank << (*it).flopHand3.card1.suit << endl;
			}
			if ((*it).finalBetRound >= 2)
			{
				file << "Turn : " << (*it).turnHand.card1.rank << (*it).turnHand.card1.suit << endl;
			}
			if ((*it).finalBetRound >= 3)
			{
				file << "River: " << (*it).riverHand.card1.rank << (*it).riverHand.card1.suit << endl;
			}
			file << "Number of players : " << (*it).players.size() << endl;
			for (itt=(*it).players.begin();itt!=(*it).players.end();++itt)
			{
				file << "	" << (*itt).player.name << " seats at seat " << (*itt).seat << " with balance $" << (*itt).balance << endl;
				if ((*itt).preflopAction.size() > 0) file << "		Preflop actions : " << endl;
				for (itAction=(*itt).preflopAction.begin();itAction!=(*itt).preflopAction.end();++itAction)
				{
					file << "			" << (*itAction).type << " " << (*itAction).size << endl; 
				}
				if ((*itt).flopAction.size() > 0) file << "		Flop actions : " << endl;
				for (itAction=(*itt).flopAction.begin();itAction!=(*itt).flopAction.end();++itAction)
				{
					file << "			" << (*itAction).type << " " << (*itAction).size << endl; 
				}
				if ((*itt).turnAction.size() > 0) file << "		Turn actions : " << endl;
				for (itAction=(*itt).turnAction.begin();itAction!=(*itt).turnAction.end();++itAction)
				{
					file << "			" << (*itAction).type << " " << (*itAction).size << endl; 
				}
				if ((*itt).riverAction.size() > 0) file << "		River actions : " << endl;
				for (itAction=(*itt).riverAction.begin();itAction!=(*itt).riverAction.end();++itAction)
				{
					file << "			" << (*itAction).type << " " << (*itAction).size << endl; 
				}
				if ((*itt).handKnown)
				{
					file << "		Hands : " << (*itt).hand.card1.rank << (*itt).hand.card1.suit;
					file << " " << (*itt).hand.card2.rank << (*itt).hand.card2.suit << endl;
					nrofhands++;
				}
			}
		}

		file << "Number of hands at showdown : " << nrofhands << endl;

		file.close();
	}

	static int exportToFile(vector<HandHistory> &history, string filename)
	{
		int nrofhands = 0;
		ofstream file;
		file.open(filename, fstream::app);

		file << history.size() << endl;

		vector<HandHistory>::iterator it;
		vector<PlayerHistory>::iterator itt;
		vector<Action>::iterator itAction;
		for (it=history.begin();it!=history.end();++it)
		{
			file << (*it).finalBetRound << endl;
			if ((*it).finalBetRound >= 1)
			{
				file << (*it).flopHand1.card1.rank << (*it).flopHand1.card1.suit;
				file << (*it).flopHand2.card1.rank << (*it).flopHand2.card1.suit;
				file << (*it).flopHand3.card1.rank << (*it).flopHand3.card1.suit;
			}
			if ((*it).finalBetRound >= 2)
			{
				file << (*it).turnHand.card1.rank << (*it).turnHand.card1.suit;
			}
			if ((*it).finalBetRound >= 3)
			{
				file << (*it).riverHand.card1.rank << (*it).riverHand.card1.suit;
			}
			file << endl;
			file << it->buttonSeat << endl;
			file << (*it).players.size() << endl;
			for (itt=(*it).players.begin();itt!=(*it).players.end();++itt)
			{
				file << (*itt).player.name << endl;
				file << (*itt).balance << endl;
				file << (*itt).position << endl;
				file << (*itt).preflopAction.size() << endl;
				for (itAction=(*itt).preflopAction.begin();itAction!=(*itt).preflopAction.end();++itAction)
				{
					file << (*itAction).type << " " << (*itAction).size << endl; 
				}
				file << (*itt).flopAction.size() << endl;
				for (itAction=(*itt).flopAction.begin();itAction!=(*itt).flopAction.end();++itAction)
				{
					file << (*itAction).type << " " << (*itAction).size << endl; 
				}
				file << (*itt).turnAction.size() << endl;
				for (itAction=(*itt).turnAction.begin();itAction!=(*itt).turnAction.end();++itAction)
				{
					file << (*itAction).type << " " << (*itAction).size << endl; 
				}
				file << (*itt).riverAction.size() << endl;
				for (itAction=(*itt).riverAction.begin();itAction!=(*itt).riverAction.end();++itAction)
				{
					file << (*itAction).type << " " << (*itAction).size << endl; 
				}
				file << (*itt).handKnown << endl;
				if ((*itt).handKnown)
				{
					file << (*itt).hand.card1.rank << (*itt).hand.card1.suit;
					file << (*itt).hand.card2.rank << (*itt).hand.card2.suit << endl;
					nrofhands++;
				}
			}
		}

		file.close();

		return nrofhands;
	}

	static std::vector<HandHistory> importFromFile(FILE *f, int historyNumber)
	{
		FILE *logfile = fopen("parserlog", "a");
		std::vector<HandHistory> res;

		char s[100];
		for (int i = 0; i < historyNumber; ++i)
		{
			HandHistory handhistory;
			fgets(s, sizeof(s), f);
			sscanf(s,"%d", &handhistory.finalBetRound);


			if (handhistory.finalBetRound >= 1)
			{
				fgets(s, sizeof(s), f);
				sscanf(s,"%c%c%c%c%c%c", &handhistory.flopHand1.card1.rank, &handhistory.flopHand1.card1.suit, 
					&handhistory.flopHand2.card1.rank, &handhistory.flopHand2.card1.suit, 
					&handhistory.flopHand3.card1.rank, &handhistory.flopHand3.card1.suit);
			}
			if (handhistory.finalBetRound >= 2)
			{
				//sscanf(s,"*%c*%c*%c*%c*%c*%c%c%c%", &handhistory.turnHand.card1.rank, &handhistory.turnHand.card1.suit);
				handhistory.turnHand.card1.rank = s[6];
				handhistory.turnHand.card1.suit = s[7];
			}
			if (handhistory.finalBetRound >= 3)
			{
				//sscanf(s,"%c%c%", &handhistory.flopHand1.card1.rank, &handhistory.flopHand1.card1.suit);
				handhistory.riverHand.card1.rank = s[8];
				handhistory.riverHand.card1.suit = s[9];
			}

			fgets(s, sizeof(s), f);

			int playernum;
			fgets(s, sizeof(s), f);
			sscanf(s,"%d", &playernum);
			
			std::vector<PlayerHistory> p;
			for (int j = 0; j < playernum; ++j)
			{
				PlayerHistory player;
				fgets(s, sizeof(s), f);
				//char bla[100];
				//sscanf(s,"%s\n", bla);
				player.player.name = s;
				player.player.name.erase(player.player.name.find_last_not_of(" \n\r\t")+1);
				//printf("%s\n", player.player.name.c_str());
				
				fgets(s, sizeof(s), f);
				sscanf(s,"%lf", &player.balance);
				if (player.balance > 100 || player.balance < 0)
				{
					printf("%s\n", player.player.name.c_str());
					printf("Error in balance %lf\n", player.balance);
					fprintf(logfile,"balance error\n");
				}

				fgets(s, sizeof(s), f);
				sscanf(s,"%d", &player.position);

				if (player.position > 2 || player.position < -3)
				{
					//printf("%s\n", s);
					printf("%s\n", player.player.name.c_str());
					printf("%d\n", handhistory.buttonSeat);
					printf("Error in position %d\n", player.position);
					fprintf(logfile,"error in position %d %d\n", player.position, handhistory.buttonSeat);
					player.position = -3;
				}

				int actions;
				fgets(s, sizeof(s), f);
				sscanf(s,"%d", &actions);
				if (actions > 20 || actions < 0)
				{
					printf("error in actions: %d\n", actions);
				}

				Action action;
				for (int k = 0; k < actions; k++)
				{
					fgets(s, sizeof(s), f);
					sscanf(s,"%s %lf", &action.type, &action.size);
					player.preflopAction.push_back(action);
				}

				fgets(s, sizeof(s), f);
				sscanf(s,"%d", &actions);
			
				for (int k = 0; k < actions; k++)
				{
					fgets(s, sizeof(s), f);
					sscanf(s,"%s %lf", &action.type, &action.size);
					player.flopAction.push_back(action);
				}

				fgets(s, sizeof(s), f);
				sscanf(s,"%d", &actions);
				for (int k = 0; k < actions; k++)
				{
					fgets(s, sizeof(s), f);
					sscanf(s,"%s %lf", &action.type, &action.size);
					player.turnAction.push_back(action);
				}

				fgets(s, sizeof(s), f);
				sscanf(s,"%d", &actions);
				for (int k = 0; k < actions; k++)
				{
					fgets(s, sizeof(s), f);
					sscanf(s,"%s %lf", &action.type, &action.size);
					player.riverAction.push_back(action);
				}

				fgets(s, sizeof(s), f);
				sscanf(s,"%d", &player.handKnown);
				if (player.handKnown)
				{
					fgets(s, sizeof(s), f);
					sscanf(s,"%c%c%c%c", &player.hand.card1.rank, &player.hand.card1.suit, &player.hand.card2.rank, &player.hand.card2.suit);
				}

				handhistory.players.push_back(player);
			}
			
			res.push_back(handhistory);
		}
		printf("End parsing\n");
		fclose(logfile);
		return res;
	}
};

// abstract Parser class, for parsing handhistory files
class Parser
{
public:
	virtual vector<HandHistory> parse(string filename) = 0;
};

class OngameParser : public Parser
{
public:
	vector<HandHistory> parse(string filename)
	{
		cout << "Begin parsing file: " << filename << endl;
		ifstream file;
		file.open(filename);
		
		vector<HandHistory> result;
		HandHistory actualhand;
		PlayerHistory actualPlayerHistory;
		Player actualPlayer;
		string line;
		int round = 0;
		int buttonSeat;
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
				actualhand.id = string(what[1].first,what[1].second);
				actualhand.finalBetRound = 0;
			}
			// Found end of hand
			else if (regex_match(line,handend))
			{
				actualhand.buttonSeat = buttonSeat;
				HandHistoryUtils::setPlayersPosition(actualhand, buttonSeat);
				result.push_back(actualhand);
				actualhand.players.clear();
				round = 0;
			}
			// Found the table
			else if (regex_search(line, what, table, flags))
			{
				actualhand.table = string(what[1].first,what[1].second);
			}
			// Found a player
			else if (regex_search(line, what, player, flags))
			{
				actualPlayer.name = string(what[2].first,what[2].second);
				if ( !HandHistoryUtils::playerExists(actualhand, actualPlayer.name) )
				{
					actualPlayerHistory.player = actualPlayer;
					actualPlayerHistory.balance = atof(string(what[3].first,what[3].second).c_str());
					actualPlayerHistory.seat = atoi(string(what[1].first,what[1].second).c_str());
					actualPlayerHistory.preflopAction.clear();
					actualPlayerHistory.flopAction.clear();
					actualPlayerHistory.turnAction.clear();
					actualPlayerHistory.riverAction.clear();
					actualPlayerHistory.handKnown = false;
					actualhand.players.push_back(actualPlayerHistory);
				}
			}
			// Found a call action
			else if (regex_search(line, what, call, flags))
			{
				Action tempAction;
				tempAction.size = atof(string(what[2].first,what[2].second).c_str());
				tempAction.type = 'c';
				string player_name = string(what[1].first,what[1].second);
				HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, player_name, round);
			}
			// Found a fold action
			else if (regex_search(line, what, fold, flags))
			{
				Action tempAction;
				tempAction.size = 0;
				tempAction.type = 'f';
				string player_name = string(what[1].first,what[1].second);
				HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, player_name, round);
			}
			// Found a bet action
			else if (regex_search(line, what, bet, flags))
			{
				Action tempAction;
				tempAction.size = atof(string(what[2].first,what[2].second).c_str());
				tempAction.type = 'r';
				string player_name = string(what[1].first,what[1].second);
				HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, player_name, round);
			}
			// Found a check action
			else if (regex_search(line, what, check, flags))
			{
				Action tempAction;
				tempAction.size = 0;
				tempAction.type = 'x';
				string player_name = string(what[1].first,what[1].second);
				HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, player_name, round);
			}
			// Found a raise action
			else if (regex_search(line, what, raise, flags))
			{
				Action tempAction;
				tempAction.size = atof(string(what[2].first,what[2].second).c_str());
				tempAction.type = 'r';
				string player_name = string(what[1].first,what[1].second);
				HandHistoryUtils::addActiontoPlayer(actualhand, tempAction, player_name, round);
			}
			// Found dealing flop
			else if (regex_search(line, what, flop, flags))
			{
				round = 1;
				actualhand.finalBetRound = 1;
				actualhand.flopHand1.card1.rank = string(what[1].first,what[1].second).c_str()[0];
				actualhand.flopHand1.card1.suit = string(what[2].first,what[2].second).c_str()[0];
				actualhand.flopHand2.card1.rank = string(what[3].first,what[3].second).c_str()[0];
				actualhand.flopHand2.card1.suit = string(what[4].first,what[4].second).c_str()[0];
				actualhand.flopHand3.card1.rank = string(what[5].first,what[5].second).c_str()[0];
				actualhand.flopHand3.card1.suit = string(what[6].first,what[6].second).c_str()[0];
			}
			// Found dealing turn
			else if (regex_search(line, what, turn, flags))
			{
				round = 2;
				actualhand.finalBetRound = 2;
				actualhand.turnHand.card1.rank = string(what[1].first,what[1].second).c_str()[0];
				actualhand.turnHand.card1.suit = string(what[2].first,what[2].second).c_str()[0];
			}
			// Found dealing river
			else if (regex_search(line, what, river, flags))
			{
				round = 3;
				actualhand.finalBetRound = 3;
				actualhand.riverHand.card1.rank = string(what[1].first,what[1].second).c_str()[0];
				actualhand.riverHand.card1.suit = string(what[2].first,what[2].second).c_str()[0];
			}
			else if (regex_search(line, what, button, flags))
			{
				buttonSeat = atoi(string(what[1].first,what[1].second).c_str());
			}
			// Found a player's hands at showdown
			if (regex_search(line, what, showdown, flags))
			{
				Hand tempHand;
				tempHand.card1.rank = string(what[2].first,what[2].second).c_str()[0];
				tempHand.card1.suit = string(what[3].first,what[3].second).c_str()[0];
				tempHand.card2.rank = string(what[4].first,what[4].second).c_str()[0];
				tempHand.card2.suit = string(what[5].first,what[5].second).c_str()[0];

				HandHistoryUtils::setPlayersHand(actualhand, string(what[1].first,what[1].second), tempHand);
			}
		}

		file.close();
		cout << "End parsing file" << endl;
		return result;
	}
};