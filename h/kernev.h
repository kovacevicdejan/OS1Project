#ifndef KERNEV_H_
#define KERNEV_H_

#include "event.h"
#include "pcb.h"

class KernelEv {
public:
    KernelEv(IVTNo ivtNo);
    void wait();
    void signal();
    ~KernelEv();

private:
    int value;
    PCB* blockedPCB;
    PCB* creatorPCB;
    IVTNo ivtNo;
};

#endif /* EVENT_H_ */