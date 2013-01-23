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

double PlayerHistory::getMax(vector<Action>& actions)
{
	double max = 0;
	for (int i = 0; i < actions.size(); ++i)
	{
		if (actions[i].getType() == 'r' || actions[i].getType() == 'c')
		{
			if (max < actions[i].getSize()) max = actions[i].getSize();
		}
	}

	return max;
}

double PlayerHistory::getBalanceInRound(int round)
{
	double playerBalance = balance;
	if (round > 1)
	{
		playerBalance -= getMax(preflopAction);
	}

	if (round > 2)
	{
		playerBalance -= getMax(flopAction);
	}

	if (round > 3)
	{
		playerBalance -= getMax(turnAction);
	}

	return playerBalance;
}

bool PlayerHistory::isFolded()
{
	bool folded = false;

	for (int j = 0; j < getPreflopAction().size(); ++j)
	{
		Action action = getPreflopAction()[j];
		if (action.getType() == 'f')
		{
			folded = true;
			break;
		}
	}
	for (int j = 0; j < getFlopAction().size(); ++j)
	{
		Action action = getFlopAction()[j];
		if (action.getType() == 'f')
		{
			folded = true;
			break;
		}
	}
	for (int j = 0; j < getTurnAction().size(); ++j)
	{
		Action action = getTurnAction()[j];
		if (action.getType() == 'f')
		{
			folded = true;
			break;
		}
	}
	for (int j = 0; j < getRiverAction().size(); ++j)
	{
		Action action = getRiverAction()[j];
		if (action.getType() == 'f')
		{
			folded = true;
			break;
		}
	}

	return folded;
}
bool PlayerHistory::isFoldedInRound(vector<Action>& actions)
{
	for (int i = 0; i < actions.size(); ++i)
	{
		if (actions[i].getType() == 'f') return true;		
	}

	return false;
}