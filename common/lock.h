#ifndef _LOCK_H_20130423102352
#define _LOCK_H_20130423102352

#include <Windows.h>

class CLock
{
public:
    CLock();
    ~CLock();

    void lock();
    void unlock();

private:
    CRITICAL_SECTION m_cs;
};


#endif