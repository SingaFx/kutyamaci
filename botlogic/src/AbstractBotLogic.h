#pragma once

#include "Action.h"
#include "currentGameInfo.h"
#include "rangeFunctions.h"

class AbstractBotLogic
{
public:
    //AbstractBotLogic();
    //virtual ~AbstractBotLogic() = 0;

    virtual PlayerRange calculateRange(int id, CurrentGameInfo&, PlayerRange&) = 0;
    virtual Action makeDecision(CurrentGameInfo&, vector<PlayerRange>&) = 0;
};
