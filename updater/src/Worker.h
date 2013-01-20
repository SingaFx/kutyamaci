#pragma once

#include <string>
#include <boost/thread.hpp>

//Worker.h is already included into in live_updater.h
class LiveUpdater;
using namespace std;

class Worker
{
private:

    string hFind;
    LiveUpdater* updater;
    boost::mutex& updateLock;
    static int threadCounter;

public:

    Worker(string, LiveUpdater*, boost::mutex&);

    void operator()();
};