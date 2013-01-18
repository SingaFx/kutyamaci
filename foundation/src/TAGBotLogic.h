#pragma once

#include "AbstractBotLogic.h"

class TAGBotLogic : public AbstractBotLogic
{
public:

    TAGBotLogic();
    virtual ~TAGBotLogic();

    virtual PlayerRange& calculateRange(string name, const CurrentGameInfo&, const PlayerRange&);
    virtual Action makeDecision(const CurrentGameInfo&, const PlayerRange&);
};