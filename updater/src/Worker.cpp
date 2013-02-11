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
    //Logger& logger = Logger::getLogger(LOGGER_TYPE::HAND_HISTORY_PARSER);
    //logger.logExp("Started thread worker number: ", threadCounter, LOGGER_TYPE::HAND_HISTORY_PARSER);
    //logger.logExp("Thread watches: ", hFind.c_str(), LOGGER_TYPE::HAND_HISTORY_PARSER);
    threadCounter++;

    ifstream fileHandle;

    BwinPartyParser parser(PARSER_TYPE::LIVE_PARSER, fileHandle);

    vector<HandHistory> history;

    parser.openFileForParsing(hFind);

    while(1)
    {
        parser.openFileForParsing(hFind);

        history = parser.parse();

        boost::mutex::scoped_lock lock1(updateLock);
        updater->update(history);

        history.clear();

        parser.closeFileAfterParsing();

        Sleep(5000);
    }
}   