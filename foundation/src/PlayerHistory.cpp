#include "PlayerHistory.h"

PlayerHistory::PlayerHistory()
 : playerName("")
 , seat(-1)
 , position(-100)
 , balance(0)
 , handKnown(false)
 , hand()
 , preflopAction()
 , flopAction()
 , turnAction()
 , riverAction()
{
}

PlayerHistory::PlayerHistory(string p , int s, int pos, double b, bool hk, Hand h , vector<Action> pre, vector<Action> flop, vector<Action> turn , vector<Action> river)
 : playerName(p)
 , seat(s)
 , position(pos)
 , handKnown(hk)
 , hand(h)
 , preflopAction(pre)
 , flopAction(flop)
 , turnAction(turn)
 , riverAction(river)
{
}

void PlayerHistory::setPlayerName(string p)
{
    playerName = p;
}

const string&  PlayerHistory::getPlayerName() const
{
    return playerName;
}

void PlayerHistory::setSeat(int s)
{
    seat = s;
}

int PlayerHistory::getSeat() const
{
    return seat;
}

void PlayerHistory::setPosition(int pos)
{
    position = pos;
}

int PlayerHistory::getPosition() const
{
    return position;
}

void PlayerHistory::setBalance(double b)
{
    balance = b;
}

double PlayerHistory::getBalance() const
{
    return balance;
}

void PlayerHistory::setHandKnown(bool hk)
{
    handKnown = hk;
}

bool PlayerHistory::isHandKnown() const
{
    return handKnown;
}

void PlayerHistory::setHand(Hand h)
{
    hand = h;
}

const Hand& PlayerHistory::getHand() const
{
    return hand;
}

void PlayerHistory::setPreflopAction(vector<Action> pre)
{
    preflopAction = pre;
}

const vector<Action>& PlayerHistory::getPreflopAction() const
{
    return preflopAction;
}

vector<Action>& PlayerHistory::getPreflopAction()
{
    return preflopAction;
}

void PlayerHistory::setFlopAction(vector<Action> flop)
{
    flopAction = flop;
}

const vector<Action>& PlayerHistory::getFlopAction() const
{
    return flopAction;
}

vector<Action>& PlayerHistory::getFlopAction()
{
    return flopAction;
}

void PlayerHistory::setTurnAction(vector<Action> turn)
{
    turnAction = turn;
}

const vector<Action>& PlayerHistory::getTurnAction() const
{
    return turnAction;
}

vector<Action>& PlayerHistory::getTurnAction()
{
    return turnAction;
}

void PlayerHistory::setRiverAction(vector<Action> river)
{
    riverAction = river;
}

const vector<Action>& PlayerHistory::getRiverAction() const
{
    return riverAction;
}

vector<Action>& PlayerHistory::getRiverAction()
{
    return riverAction;
}