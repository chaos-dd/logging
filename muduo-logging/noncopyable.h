
#ifndef MUDUO_NONCOPYABLE_H
#define MUDUO_NONCOPYABLE_H
namespace muduo
{

    class noncopyable
    {
    protected:
        noncopyable() {}
        ~noncopyable() {}
    private:  // emphasize the following members are private
        noncopyable(const noncopyable&);
        noncopyable& operator=(const noncopyable&);

    };

}

#endif