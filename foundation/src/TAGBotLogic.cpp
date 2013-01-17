#include "TAGBotLogic.h"

TAGBotLogic::TAGBotLogic()
 : AbstractBotLogic()
{
}

TAGBotLogic::~TAGBotLogic()
{
}

PlayerRange& TAGBotLogic::calculateRange(const CurrentGameInfo& cgi)
{
    PlayerRange playerRange;

    return playerRange;
}

Action TAGBotLogic::makeDecision(const CurrentGameInfo&, const PlayerRange&)
{
    return Action();
}