#pragma once

#include "Card.h"
#include "PlayerHistory.h"
#include <string>

using namespace std;

class HandHistory
{
private:

    string id;
	string table;
	vector<PlayerHistory> playerHistories;
	Card flopCard1, flopCard2, flopCard3, turnCard, riverCard;
	int finalBetRound;
	int buttonSeat;
public:

    HandHistory();
    HandHistory(string, string, vector<PlayerHistory>, Card, Card, Card, Card, Card, int, int);

    void setId(string);
    const string& getId() const;

    void setTable(string);
    const string& getTable() const;

    void setPlayerHistories(vector<PlayerHistory>);
    const vector<PlayerHistory>& getPlayerHistories() const;
    vector<PlayerHistory>& getPlayerHistories();

    void setFlopCard1(Card);
    const Card& getFlopCard1() const;

    void setFlopCard2(Card);
    const Card& getFlopCard2() const;

    void setFlopCard3(Card);
    const Card& getFlopCard3() const;

    void setTurnCard(Card);
    const Card& getTurnCard() const;

    void setRiverCard(Card);
    const Card& getRiverCard() const;

    void setFinalBetRound(int);
    int getFinalBetRound() const;

    void setButtonSeat(int);
    int getButtonSeat() const;
};