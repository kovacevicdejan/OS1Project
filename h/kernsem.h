#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "thread.h"
#include "list.h"
#include "timelist.h"

extern volatile List allKernelSem;
extern volatile int lockTickCounter;

class KernelSem {
private:
    TimeList blockedPCB;
    int value;
public:
    KernelSem(int init);
    int wait(Time maxTimeToWait);
    void signal();
    int val() const;
    void releaseWaiting();
    static void tick();
    ~KernelSem();
};

#endif /* KERNSEM_H_ */
