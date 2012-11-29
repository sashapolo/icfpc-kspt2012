#include "stdinclude.h"
#include "Utilities.h"

int msecDiff(timespec& t1,timespec& t2)
{
    return abs((t2.tv_sec-t1.tv_sec)*1000+(t2.tv_nsec-t1.tv_nsec)/1000);
};