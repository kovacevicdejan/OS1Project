#include "ivtentry.h"
#include "kernev.h"
#include "flags.h"
#include "dos.h"

volatile IVTEntry* IVTable[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt interruptRoutine){
    lock;
    newRoutine = interruptRoutine;
    this->ivtNo = ivtNo;
    this->myKernelEv = 0;
    IVTable[ivtNo] = this;
    unlock;
}

void IVTEntry::setMyKernelEv(KernelEv* myKernelEv) {
    disable_interrupts;
    this->myKernelEv = myKernelEv;
    oldRoutine = getvect(ivtNo);
    setvect(ivtNo, newRoutine);
    enable_interrupts;
}

IVTEntry* IVTEntry::getEntry(IVTNo ivtNo) {
    return (IVTEntry*)IVTable[ivtNo];
}

void IVTEntry::signal() {
    myKernelEv->signal();
}

void IVTEntry::callOldRoutine() {
    (*oldRoutine)();
}

void IVTEntry::setOldRoutine() {
    disable_interrupts;
    setvect(ivtNo, oldRoutine);
    myKernelEv = 0;
    IVTable[ivtNo] = 0;
    enable_interrupts;
}

IVTEntry::~IVTEntry() {
    setOldRoutine();
}