#include "header.h"

int main()
{
    struct Info info;
    initInfo(&info);
    createMutex(&info);
    runThreads(&info);
    waitThreads(&info);
    return 0;
}
