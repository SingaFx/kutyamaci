#include "HandHistoryUtils.h"
#include <iostream>
#include <boost/regex.hpp>

//checks whether player exists and player history
bool HandHistoryUtils::playerExists(HandHistory &history, string playername)
{
	vector<PlayerHistory>::iterator it;
    vector<PlayerHistory>& playerHistories = history.getPlayerHistories();
	for (it = playerHistories.begin(); it != playerHistories.end(); ++it)
	{
		if ((*it).getPlayerName() == playername)
		{
			return true;
		}
	}

	return false;
}

void HandHistoryUtils::addActiontoPlayer(HandHistory &history, Action &action, string playername, int round)
{
	vector<PlayerHistory>::iterator it;
    vector<PlayerHistory>& playerHistories = history.getPlayerHistories();
	for (it = playerHistories.begin(); it != playerHistories.end(); ++it)
	{
		if ((*it).getPlayerName() == playername)
		{
			switch(round)
			{
			case 0:
				(*it).getPreflopAction().push_back(action);
				break;
			case 1:
				(*it).getFlopAction().push_back(action);
				break;
			case 2:
				(*it).getTurnAction().push_back(action);
				break;
			case 3:
				(*it).getRiverAction().push_back(action);
				break;
			}
			break;
		}
	}
}

void HandHistoryUtils::setPlayersHand(HandHistory &history, string playername, Hand &hand)
{
	vector<PlayerHistory>::iterator it;
    vector<PlayerHistory>& playerHistories = history.getPlayerHistories();
	for (it = playerHistories.begin(); it != playerHistories.end(); ++it)
	{
		if ((*it).getPlayerName() == playername)
		{
			(*it).setHandKnown(true);
			(*it).setHand(hand);
			break;
		}
	}
}

void HandHistoryUtils::setPlayersPosition(HandHistory &history, int buttonSeat)
{
    vector<PlayerHistory>& playerHistories = history.getPlayerHistories();
	int nr = playerHistories.size();
	int buttonnr = 0;

	for (int i = 0; i < nr; ++i)
	{
		if (playerHistories[i].getSeat() == buttonSeat)
		{
			buttonnr = i;
			playerHistories[i].setPosition(0);
		}
	}

	for (int i = buttonnr+1; i < nr; ++i)
	{
		switch (i-buttonnr)
		{
		case 1:
		case 2:
			playerHistories[i].setPosition(i-buttonnr);
			break;
		default:
			playerHistories[i].setPosition(i-nr-buttonnr);
			break;
		}
	}

	for (int i = 0; i < buttonnr; ++i)
	{
		switch(i-buttonnr)
		{
		case -5:
			playerHistories[i].setPosition(1);
			break;
		case -4:
			playerHistories[i].setPosition(2);
			break;
		default:
			playerHistories[i].setPosition(i - buttonnr);
			break;
		}
	}
}

void HandHistoryUtils::detailedExportToFile(vector<HandHistory>& histories, string filename)
{
	int nrofhands = 0;
	ofstream file;
	file.open(filename, fstream::app);

	file << "Number of hands : " << histories.size() << endl;

	vector<HandHistory>::iterator it;

	for (it = histories.begin(); it != histories.end(); ++it)
	{
		file << "Hand " << (*it).getId() << endl;
		file << "Table : " << (*it).getTable() << endl;

		if ((*it).getFinalBetRound() >= 1)
		{
			file << "Flop : " << (*it).getFlopCard1().getRank() << (*it).getFlopCard1().getSuit() << " ";
			file << (*it).getFlopCard2().getRank() << (*it).getFlopCard2().getSuit() << " ";
			file << (*it).getFlopCard3().getRank() << (*it).getFlopCard3().getSuit() << endl;
		}

		if ((*it).getFinalBetRound() >= 2)
		{
			file << "Turn : " << (*it).getTurnCard().getRank() << (*it).getTurnCard().getSuit() << endl;
		}

		if ((*it).getFinalBetRound() >= 3)
		{
			file << "River: " << (*it).getRiverCard().getRank() << (*it).getRiverCard().getSuit() << endl;
		}

        vector<PlayerHistory>& playerHistories = (*it).getPlayerHistories();

		file << "Number of players : " << playerHistories.size() << endl;
        vector<PlayerHistory>::iterator itt;
        vector<Action>::iterator itAction;
		for (itt = playerHistories.begin(); itt != playerHistories.end();++itt)
		{
			file << "	" << (*itt).getPlayerName() << " seats at seat " << (*itt).getSeat() << " with balance $" << (*itt).getBalance() << endl;
            vector<Action>& preflopAction = (*itt).getPreflopAction();
			if (preflopAction.size() > 0)
            {
                file << "		Preflop actions : " << endl;

			    for (itAction = preflopAction.begin(); itAction != preflopAction.end(); ++itAction)
			    {
				    file << "			" << (*itAction).getType() << " " << (*itAction).getSize() << endl;
			    }
            }

            vector<Action>& flopAction = (*itt).getFlopAction();
			if (flopAction.size() > 0)
            {
                file << "		Flop actions : " << endl;
			    for (itAction =flopAction.begin();itAction != flopAction.end(); ++itAction)
			    {
				    file << "			" << (*itAction).getType() << " " << (*itAction).getSize() << endl;
			    }
            }

            vector<Action>& turnAction = (*itt).getTurnAction();
			if (turnAction.size() > 0)
            {
                file << "		Turn actions : " << endl;
			    for (itAction = turnAction.begin(); itAction != turnAction.end(); ++itAction)
			    {
				    file << "			" << (*itAction).getType() << " " << (*itAction).getSize() << endl;
			    }
            }

            vector<Action>& riverAction = (*itt).getRiverAction();
			if (riverAction.size() > 0)
            {
                file << "		River actions : " << endl;
			    for (itAction = riverAction.begin(); itAction != riverAction.end(); ++itAction)
			    {
				    file << "			" << (*itAction).getType() << " " << (*itAction).getSize() << endl;
			    }
             }

			if ((*itt).isHandKnown())
			{
				file << "		Hands : " << (*itt).getHand().getCard1().getRank() << (*itt).getHand().getCard1().getSuit();
				file << " " << (*itt).getHand().getCard2().getRank() << (*itt).getHand().getCard2().getSuit() << endl;
				nrofhands++;
			}
		}
	}

	file << "Number of hands at showdown : " << nrofhands << endl;

	file.close();
}

 int HandHistoryUtils::exportToFile(vector<HandHistory> &history, string filename)
{
	int nrofhands = 0;
	ofstream file;
	file.open(filename, fstream::app);

	file << history.size() << endl;

	vector<HandHistory>::iterator it;
	vector<PlayerHistory>::iterator itt;
	vector<Action>::iterator itAction;

	for (it = history.begin(); it!=history.end(); ++it)
	{
        int finalBetRound = (*it).getFinalBetRound();
        file << finalBetRound << endl;
		if (finalBetRound >= 1)
		{
			file << (*it).getFlopCard1().getRank() << (*it).getFlopCard1().getSuit();
			file << (*it).getFlopCard2().getRank() << (*it).getFlopCard2().getSuit();
			file << (*it).getFlopCard3().getRank() << (*it).getFlopCard3().getSuit();
		}

		if (finalBetRound >= 2)
		{
			file << (*it).getTurnCard().getRank() << (*it).getTurnCard().getSuit();
		}

		if (finalBetRound >= 3)
		{
			file << (*it).getRiverCard().getRank() << (*it).getRiverCard().getSuit();
		}

		file << endl;
		file << (*it).getButtonSeat() << endl;
        vector<PlayerHistory> playerHistories = (*it).getPlayerHistories();
		file << playerHistories.size() << endl;

		for (itt = playerHistories.begin(); itt != playerHistories.end(); ++itt)
		{
			file << (*itt).getPlayerName() << endl;
			file << (*itt).getBalance() << endl;
			file << (*itt).getPosition() << endl;
            vector<Action>& preflopAction = (*itt).getPreflopAction();
			file << preflopAction.size() << endl;
			for (itAction = preflopAction.begin(); itAction != preflopAction.end(); ++itAction)
			{
				file << (*itAction).getType() << " " << (*itAction).getSize() << endl;
			}

			vector<Action>& flopAction = (*itt).getFlopAction();
            file << flopAction.size() << endl;
			for (itAction = flopAction.begin(); itAction != flopAction.end(); ++itAction)
			{
				file << (*itAction).getType() << " " << (*itAction).getSize() << endl;
			}

            vector<Action>& turnAction = (*itt).getTurnAction();
			file << turnAction.size() << endl;
			for (itAction = turnAction.begin(); itAction != turnAction.end(); ++itAction)
			{
				file << (*itAction).getType() << " " << (*itAction).getSize() << endl;
			}

			vector<Action>& riverAction = (*itt).getRiverAction();
            file <<  riverAction.size() << endl;
			for (itAction = riverAction.begin(); itAction != riverAction.end(); ++itAction)
			{
				file << (*itAction).getType() << " " << (*itAction).getSize() << endl;
			}

			file << (*itt).isHandKnown() << endl;
			if ((*itt).isHandKnown())
			{
				file << (*itt).getHand().getCard1().getRank() << (*itt).getHand().getCard1().getSuit();
				file << (*itt).getHand().getCard2().getRank() << (*itt).getHand().getCard2().getSuit() << endl;
				nrofhands++;
			}
		}
	}

	file.close();

	return nrofhands;
}

std::vector<HandHistory> HandHistoryUtils::importFromFile(FILE *f, int historyNumber)
{
	FILE *logfile = fopen("parserlog", "a");
	std::vector<HandHistory> res;

	char s[100];
	for (int i = 0; i < historyNumber; ++i)
	{
		HandHistory handhistory;
		fgets(s, sizeof(s), f);
        int finalBetRound = -1;
		sscanf(s, "%d", &finalBetRound);
        handhistory.setFinalBetRound(finalBetRound);
		if (handhistory.getFinalBetRound() >= 1)
		{
			fgets(s, sizeof(s), f);
            char f1r, f1s, f2r, f2s, f3r, f3s;
			sscanf(s,"%c%c%c%c%c%c", &f1r, &f1s,&f2r, &f2s,&f3r, &f3s);
            handhistory.setFlopCard1(Card(f1r, f1s));
            handhistory.setFlopCard1(Card(f2r, f2s));
            handhistory.setFlopCard1(Card(f3r, f3s));
		}
		if (handhistory.getFinalBetRound() >= 2)
		{
			//sscanf(s,"*%c*%c*%c*%c*%c*%c%c%c%", &handhistory.turnHand.card1.rank, &handhistory.turnHand.card1.suit);
			//handhistory.turnHand.card1.rank = s[6];
			//handhistory.turnHand.card1.suit = s[7];
            handhistory.setTurnCard(Card(s[6], s[7]));
		}
		if (handhistory.getFinalBetRound() >= 3)
		{
			//sscanf(s,"%c%c%", &handhistory.flopHand1.card1.rank, &handhistory.flopHand1.card1.suit);
			//handhistory.riverHand.card1.rank = s[8];
			//handhistory.riverHand.card1.suit = s[9];
            handhistory.setRiverCard(Card(s[8], s[9]));
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
            string tmp(s);
            tmp.erase(tmp.find_last_not_of(" \n\r\t")+1);
			player.setPlayerName(tmp);
			//player.player.name.erase(player.player.name.find_last_not_of(" \n\r\t")+1);
			//printf("%s\n", player.player.name.c_str());

			fgets(s, sizeof(s), f);
            double balance;
			sscanf(s,"%lf", &balance);
			if (balance > 100 || balance < 0)
			{
				printf("%s\n", player.getPlayerName().c_str());
				printf("Error in balance %lf\n", player.getBalance());
				fprintf(logfile,"balance error\n");
			}

			fgets(s, sizeof(s), f);
            int position;
			sscanf(s,"%d", &position);

			if (position > 2 || position < -3)
			{
				//printf("%s\n", s);
				printf("%s\n", player.getPlayerName().c_str());
				printf("%d\n", handhistory.getButtonSeat());
				printf("Error in position %d\n", player.getPosition());
				fprintf(logfile,"error in position %d %d\n", player.getPosition(), handhistory.getButtonSeat());
				player.setPosition(-3);
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
                char type;
                double size;
				sscanf(s,"%s %lf", &type, &size);
                action.setAction(type, size);
				player.getPreflopAction().push_back(action);
			}

			fgets(s, sizeof(s), f);
			sscanf(s,"%d", &actions);

			for (int k = 0; k < actions; k++)
			{
				fgets(s, sizeof(s), f);
                char type;
                double size;
				sscanf(s,"%s %lf", &type, &size);
                action.setAction(type, size);
				player.getFlopAction().push_back(action);
			}

			fgets(s, sizeof(s), f);
			sscanf(s,"%d", &actions);
			for (int k = 0; k < actions; k++)
			{
				fgets(s, sizeof(s), f);
                char type;
                double size;
				sscanf(s,"%s %lf", &type, &size);
                action.setAction(type, size);
				player.getTurnAction().push_back(action);
			}

			fgets(s, sizeof(s), f);
			sscanf(s,"%d", &actions);
			for (int k = 0; k < actions; k++)
			{
				fgets(s, sizeof(s), f);
                char type;
                double size;
				sscanf(s,"%s %lf", &type, &size);
                action.setAction(type, size);
				player.getRiverAction().push_back(action);
			}

			fgets(s, sizeof(s), f);
            bool handKnown;
			sscanf(s,"%d", &handKnown);
            player.setHandKnown(handKnown);
			if (handKnown)
			{
				fgets(s, sizeof(s), f);
                char c1r, c1s, c2r, c2s;
				sscanf(s,"%c%c%c%c", &c1r, &c1s, &c2r, &c2s);
                player.setHand(Hand(Card(c1r, c1s), Card(c2r, c2s)));
			}

			handhistory.getPlayerHistories().push_back(player);
		}

		res.push_back(handhistory);
	}
	printf("End parsing\n");
	fclose(logfile);
	return res;
}