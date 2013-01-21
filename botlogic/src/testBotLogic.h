#pragma once

#include "AbstractBotLogic.h"

class TestBotLogic : public AbstractBotLogic //implements interface
{
public:
	
    virtual PlayerRange calculateRange(string name, CurrentGameInfo&, PlayerRange&);
    virtual Action makeDecision(CurrentGameInfo&, vector<PlayerRange>&);
};