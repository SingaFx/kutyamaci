#pragma once

#include "AbstractBotLogic.h"

class TestBotLogic : public AbstractBotLogic //implements interface
{
public:
	
    virtual PlayerRange calculateRange(int id, CurrentGameInfo&, PlayerRange&);
    virtual Action makeDecision(CurrentGameInfo&, vector<PlayerRange>&);
};