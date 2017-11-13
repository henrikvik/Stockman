#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class NonCopyable
{
public:
    NonCopyable() { }
    NonCopyable(const NonCopyable& other) = delete;
    NonCopyable* operator=(const NonCopyable& other) = delete;
    virtual ~NonCopyable() { }
};

#endif // !NONCOPYABLE_H