#include "kernev.h"
#include "flags.h"
#include "SCHEDULE.H"
#include "ivtentry.h"

KernelEv::KernelEv(IVTNo ivtNo) {
    lock;
    value = 0;
    blockedPCB = 0;
    creatorPCB = (PCB*)running;
    this->ivtNo = ivtNo;
    IVTEntry::getEntry(ivtNo)->setMyKernelEv(this);
    unlock;
}

void KernelEv::wait() {
    lock;
    if(creatorPCB == running) {
        if(value == 0) {
            running->status = BLOCKED;
            blockedPCB = (PCB*)running;
            unlock;
            dispatch();
        }
        else {
            value = 0;
            unlock;
        }
    } else {
        unlock;
    }
}

void KernelEv::signal() {
    lock;
    if(blockedPCB == 0) {
        value = 1;
        unlock;
    }
    else {
        blockedPCB->status = READY;
        Scheduler::put(blockedPCB);
        blockedPCB = 0;
        unlock;
        //dispatch();
    }
}

KernelEv::~KernelEv() {
    lock;
    blockedPCB = 0;
    creatorPCB = 0;
    IVTEntry::getEntry(ivtNo)->setOldRoutine();
    unlock;
}