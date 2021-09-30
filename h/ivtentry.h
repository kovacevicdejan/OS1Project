#ifndef IVTENTRY_H_
#define IVTENTRY_H_

typedef unsigned char IVTNo;
class KernelEv;

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...);\
IVTEntry newEntry##numEntry(numEntry, inter##numEntry);\
void interrupt inter##numEntry(...) {\
    newEntry##numEntry.signal();\
    if (callOld == 1)\
        newEntry##numEntry.callOldRoutine();\
}

typedef void interrupt (*pInterrupt)(...);

class IVTEntry {
public:
    IVTEntry(IVTNo ivtNo, pInterrupt interruptRoutine);
    void setMyKernelEv(KernelEv* myKernelEv);
    static IVTEntry* getEntry(IVTNo ivtNo);
    void signal();
    void callOldRoutine();
    void setOldRoutine();
    ~IVTEntry();
private:
    pInterrupt oldRoutine;
    pInterrupt newRoutine;
    KernelEv* myKernelEv;
    IVTNo ivtNo;
};

extern volatile IVTEntry* IVTable[256];

#endif /* IVTENTRY_H_ */