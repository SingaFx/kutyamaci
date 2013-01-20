#include "Worker.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "BwinPartyParser.h"
#include "HandHistory.h"
#include "live_updater.h"
#include "logger.h"

using namespace std;

int Worker::threadCounter = 0;

Worker::Worker(string hFind, LiveUpdater* updater, boost::mutex& lock)
 : hFind(hFind)
 , updater(updater)
 , updateLock(lock)
{
}

void Worker::operator()()
{
    Logger& logger = Logger::getLogger(LOGGER_TYPE::HAND_HISTORY_PARSER);
    logger.logExp("Started thread worker number: ", threadCounter, LOGGER_TYPE::HAND_HISTORY_PARSER);
    logger.logExp("Thread watches: ", hFind.c_str(), LOGGER_TYPE::HAND_HISTORY_PARSER);
    threadCounter++;

    while(1)
    {
        BwinPartyParser parser;
        vector<HandHistory> history = parser.parse(hFind);

        boost::mutex::scoped_lock lock1(updateLock);
        updater->update(history);

        Sleep(5000);
    }

    logger.logExp("One of the thread workers ended.", LOGGER_TYPE::HAND_HISTORY_PARSER);
}   