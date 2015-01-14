#ifndef MUDUO_BASE_ASYNCLOGGING_H
#define MUDUO_BASE_ASYNCLOGGING_H

#include <vector>
#include <memory>


#include "BlockingQueue.h"
//#include <muduo/base/BoundedBlockingQueue.h>
#include "CountDownLatch.h"

#include "Thread.h"

#include "LogStream.h"
#include "noncopyable.h"


namespace muduo
{
    using std::string;
    class AsyncLogging : noncopyable
    {
    public:

        AsyncLogging(const string& basename,
                     size_t rollSize,
                     int flushInterval = 3);

        ~AsyncLogging()
        {
            if (running_)
            {
                stop();
            }
        }

        void append(const char* logline, int len);

        void start()
        {
            running_ = true;
            thread_.start();
            latch_.wait();
        }

        void stop()
        {
            running_ = false;
            cond_.notify_one();
            thread_.join();
        }

    private:

        // declare but not define, prevent compiler-synthesized functions
        AsyncLogging(const AsyncLogging&);  // ptr_container
        void operator=(const AsyncLogging&);  // ptr_container

        void threadFunc();

        typedef muduo::detail::FixedBuffer<muduo::detail::kLargeBuffer> Buffer;
        typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
        typedef BufferVector::value_type BufferPtr;

        const int flushInterval_;
        std::atomic<bool> running_;
        string basename_;
        size_t rollSize_;
        muduo::Thread thread_;
        muduo::CountDownLatch latch_;
        std::mutex mutex_;
        std::condition_variable cond_;
        BufferPtr currentBuffer_;
        BufferPtr nextBuffer_;
        BufferVector buffers_;
    };

}
#endif  // MUDUO_BASE_ASYNCLOGGING_H
