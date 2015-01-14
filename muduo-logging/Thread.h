// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H



#include <functional>

#include <memory>
#include <thread>
#include <string>
#include <atomic>

#include "noncopyable.h"

namespace muduo
{
    using std::string;
    class Thread : noncopyable
    {
    public:
        typedef std::function<void()> ThreadFunc;

        explicit Thread(const ThreadFunc&, const string& name = string());

        explicit Thread(ThreadFunc&&, const string& name = string());

        ~Thread();

        void start();
        void join(); // return pthread_join()

        bool started() const { return started_; }
        // pthread_t pthreadId() const { return pthreadId_; }
        std::thread::id tid() const { return tid_; }
        const string& name() const { return name_; }

        static int numCreated() { return numCreated_; }

    private:
        void setDefaultName();

        bool       started_;
        bool       joined_;
        std::shared_ptr<std::thread>  pthread;
        std::thread::id tid_;
        ThreadFunc func_;
        string     name_;
        static std::atomic<int> numCreated_;
    };

}
#endif
