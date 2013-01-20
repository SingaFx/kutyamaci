#pragma once

#include "plusEVBotLogic.h"
#include "abstractbotlogic.h"
//class AbstractBotLogic;

class BotManager
{
private:

    static BotManager* botManager;

    BotManager();
    ~BotManager();

public:

    static BotManager& getBotManager();

    AbstractBotLogic* getPluggableBot();
};