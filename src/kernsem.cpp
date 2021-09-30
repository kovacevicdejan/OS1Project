#include "kernsem.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "flags.h"

volatile List allKernelSem;
volatile int lockTickCounter = 0;
 
KernelSem::KernelSem(int init) {
    lock;
    value = init;
    allKernelSem.insertLast((void*)this);
    unlock;
}

int KernelSem::wait(Time maxTimeToWait) {
    lock;
    int res = 1;
    if(--value < 0) {
        running->status = BLOCKED;
        blockedPCB.insertLast((PCB*)running, maxTimeToWait);
        unlock;
        dispatch();
        lock;
        if(running->unblockedWithSignal == 0) {
            res = 0;
            running->unblockedWithSignal = 1;
        }
        unlock;
    }
    else
        unlock;
    return res;
}

void KernelSem::signal() {
    lock;
    if(++value <= 0) {
        PCB* pcb = blockedPCB.removeFirst();
        if(pcb != 0) {
            pcb->status = READY;
            Scheduler::put(pcb);
        }
    }
    unlock;
}

int KernelSem::val() const {
    return value;
}

void KernelSem::releaseWaiting() {
    blockedPCB.tick();
    int inc = blockedPCB.removeFinishedWaiting();
    value += inc;
}

void KernelSem::tick() {
    Elem* currKernElem = allKernelSem.getFirst();
    while(currKernElem != 0) {
        ((KernelSem*)currKernElem->data)->releaseWaiting();
        currKernElem = currKernElem->next;
    }
}

KernelSem::~KernelSem() {
    lock;
    allKernelSem.removeFromList((void*)this);
    unlock;
}
