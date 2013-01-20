#pragma once

#include "plusEVBotLogic.h"
#include "abstractbotlogic.h"
//class AbstractBotLogic;

class BotManager
{
private:

    static BotManager* botManager;

	AbstractBotLogic* ourSexyBotLogic;

	AbstractBotLogic* instantiate();

    BotManager();
    ~BotManager();

public:

    static BotManager& getBotManager();

    AbstractBotLogic* getPluggableBot();
};