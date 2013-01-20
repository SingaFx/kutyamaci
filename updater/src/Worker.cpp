#include "Worker.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "BwinPartyParser.h"
#include "HandHistory.h"
#include "live_updater.h"

using namespace std;

Worker::Worker(string hFind, LiveUpdater* updater, boost::mutex& lock)
 : hFind(hFind)
 , updater(updater)
 , updateLock(lock)
{
}

void Worker::operator()()
{
    while(1)
    {
        BwinPartyParser parser;
        vector<HandHistory> history = parser.parse(hFind);

        boost::mutex::scoped_lock lock1(updateLock);
        updater->update(history);

        Sleep(5000);
    }
}   