#ifndef TIMELIST_H_
#define TIMELIST_H_

#include "list.h"
#include "pcb.h"

struct TimePCB {
    PCB* pcb;
    Time time;
    Time maxTimeToWait;
    TimePCB(PCB* p, Time t, Time mtw) {
        pcb = p;
        time = t;
        maxTimeToWait = mtw;
    }
};

class TimeList {
public:
    TimeList();
    void insertLast(PCB* pcb, Time t);
    void tick();
    int removeFinishedWaiting();
    PCB* removeFirst();
private:
    List timePCBList;
};

#endif /* TIMELIST_H_ */