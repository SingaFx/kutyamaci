#pragma once

#include "Action.h"
#include "currentGameInfo.h"
#include "rangeFunctions.h"

class AbstractBotLogic
{
public:
    AbstractBotLogic();
    virtual ~AbstractBotLogic() = 0;

    virtual PlayerRange& calculateRange(const CurrentGameInfo&) = 0;

    virtual Action makeDecision(const CurrentGameInfo&, const PlayerRange&) = 0;
};
