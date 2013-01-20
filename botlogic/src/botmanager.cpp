#include "botmanager.h"
#include "abstractbotlogic.h"
#include "plusEVBotLogic.h"

BotManager* BotManager::botManager = 0;

BotManager::BotManager()
{
	ourSexyBotLogic = new PlusEVBotLogic();
}

BotManager::~BotManager()
{
}

BotManager& BotManager::getBotManager()
{
    if (!botManager)
    {
        botManager = new BotManager();
    }

    return *botManager;
}

AbstractBotLogic* BotManager::getPluggableBot()
{
    return ourSexyBotLogic;
}