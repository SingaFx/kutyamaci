#include "HandHistory.h"

HandHistory::HandHistory()
 : id("")
 , table("")
 , bigBlind(0)
 , playerHistories()
 , flopCard1()
 , flopCard2()
 , flopCard3()
 , turnCard()
 , riverCard()
 , finalBetRound(-1)
 , buttonSeat(-1)
{
}

HandHistory::HandHistory(string i , string t, double bigBlind, vector<PlayerHistory> ph, Card f1, Card f2, Card f3, Card turn, Card river, int fbr, int bs)
 : id(i)
 , table(t)
 , bigBlind(bigBlind)
 , playerHistories(ph)
 , flopCard1(f1)
 , flopCard2(f2)
 , flopCard3(f3)
 , turnCard(turn)
 , riverCard(river)
 , finalBetRound(fbr)
 , buttonSeat(bs)
{
}

void HandHistory::setId(string i)
{
    id = i;
}

const string& HandHistory::getId() const
{
    return id;
}

void HandHistory::setTable(string t)
{
    table = t;
}

const string& HandHistory::getTable() const
{
    return table;
}

void HandHistory::setBigBlind(double bB)
{
    bigBlind = bB;
}

double HandHistory::getBigBlind() const
{
    return bigBlind;
}

void HandHistory::setPlayerHistories(vector<PlayerHistory> ph)
{
    playerHistories = ph;
}

const vector<PlayerHistory>& HandHistory::getPlayerHistories() const
{
    return playerHistories;
}

vector<PlayerHistory>& HandHistory::getPlayerHistories()
{
    return playerHistories;
}

void HandHistory::setFlopCard1(Card f1)
{
    flopCard1 = f1;
}

const Card& HandHistory::getFlopCard1() const
{
    return flopCard1;
}

void HandHistory::setFlopCard2(Card f2)
{
    flopCard2 = f2;
}

const Card& HandHistory::getFlopCard2() const
{
    return flopCard2;
}

void HandHistory::setFlopCard3(Card f3)
{
    flopCard3 = f3;
}

const Card& HandHistory::getFlopCard3() const
{
    return flopCard3;
}

void HandHistory::setTurnCard(Card turn)
{
    turnCard = turn;
}

const Card& HandHistory::getTurnCard() const
{
    return turnCard;
}

void HandHistory::setRiverCard(Card river)
{
    riverCard = river;
}

const Card& HandHistory::getRiverCard() const
{
    return riverCard;
}

void HandHistory::setFinalBetRound(int fbr)
{
    finalBetRound = fbr;
}

int HandHistory::getFinalBetRound() const
{
    return finalBetRound;
}

void HandHistory::setButtonSeat(int bs)
{
    buttonSeat = bs;
}

int HandHistory::getButtonSeat() const
{
    return buttonSeat;
}