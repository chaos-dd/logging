// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "Thread.h"

namespace muduo
{

    std::atomic<int> Thread::numCreated_;

    Thread::Thread(const ThreadFunc& func, const string& n)
        : started_(false),
        joined_(false),
        /*pthreadId_,*/
        /*tid_(pthread->get_id()),*/
        func_(func),
        name_(n)
    {
        setDefaultName();
    }


    Thread::Thread(ThreadFunc&& func, const string& n)
        : started_(false),
        joined_(false),
        /*pthreadId_(0),*/
        /*tid_(pthread->get_id()),*/
        func_(std::move(func)),
        name_(n)
    {
        setDefaultName();
    }


    Thread::~Thread()
    {
        if (started_ && !joined_)
        {
            pthread->detach();
        }
    }

    void Thread::setDefaultName()
    {
        int num = ++numCreated_;
        if (name_.empty())
        {
            char buf[32];
            sprintf(buf, "Thread%d", num);
            name_ = buf;
        }
    }

    void Thread::start()
    {
        assert(!started_);
        started_ = true;
        // FIXME: move(func_)
        
        pthread = std::make_shared<std::thread>(func_);

    }

    void Thread::join()
    {
        assert(started_);
        assert(!joined_);
        joined_ = true;
        pthread->join();
    }

}