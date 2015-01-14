#include "AsyncLogging.h"
#include "Logging.h"
#include "Timestamp.h"

#include <stdio.h>
#include <chrono>
#include <thread>
#include <random>
#include <functional>

#include "Thread.h"

using namespace std;

int kRollSize = 500 * 1000 * 1000;

muduo::AsyncLogging* g_asyncLog = NULL;

void asyncOutput(const char* msg, int len)
{
    g_asyncLog->append(msg, len);
}

void bench(bool longLog)
{
    muduo::Logger::setOutput(asyncOutput);

    int cnt = 0;
    const int kBatch = 1000;
    muduo::string empty = " ";
    muduo::string longStr(3000, 'X');
    longStr += " ";

    for (int t = 0; t < 30; ++t)
    {
        muduo::Timestamp start = muduo::Timestamp::now();
        for (int i = 0; i < kBatch; ++i)
        {
            LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
                << (longLog ? longStr : empty)
                << cnt;
            ++cnt;
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(500));

        muduo::Timestamp end = muduo::Timestamp::now();

     
        float temp = timeDifference(end, start) * 1000000 / kBatch;
        
        printf("%f\n", temp);
        
    }
}

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(1, 100);

void threadFunc()
{
    int n = 100;
    while (n-- > 0)
    {
        int time = distribution(generator);
        std::this_thread::sleep_for(chrono::milliseconds(time));
        LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz ";
    }
}

int main(int argc, char* argv[])
{
    char name[256] = "test.log";
    muduo::AsyncLogging log(name, kRollSize);
    muduo::Logger::setOutput(asyncOutput);    
        
 
    log.start();
    g_asyncLog = &log;

    bool longLog = false;
    //bench(longLog);

    const int n = 3;

    thread th[n];

    for (int i = 0; i != n; ++i)
    {
        th[i] = thread(threadFunc);
    }

    for (int i = 0; i != n; ++i)
    {
        th[i].join();
    }
}


