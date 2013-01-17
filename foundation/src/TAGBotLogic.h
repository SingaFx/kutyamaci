#pragma once

#include "AbstractBotLogic.h"

class TAGBotLogic : public AbstractBotLogic
{
public:

    TAGBotLogic();
    virtual ~TAGBotLogic();

    virtual PlayerRange& calculateRange(const CurrentGameInfo&);
    virtual Action makeDecision(const CurrentGameInfo&, const PlayerRange&);
};