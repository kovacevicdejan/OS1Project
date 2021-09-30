#include "timelist.h"
#include "flags.h"
#include "SCHEDULE.H"

TimeList::TimeList() : timePCBList() {}

void TimeList::insertLast(PCB* pcb, Time t) {
    lock;
    TimePCB* newTimePCB = new TimePCB(pcb, t, t);
    timePCBList.insertLast((void*)newTimePCB);
    unlock;
}

void TimeList::tick() {
    Elem* currTimeElem = timePCBList.getFirst();
    while(currTimeElem != 0) {
        if(((TimePCB*)currTimeElem->data)->time != 0)
            ((TimePCB*)currTimeElem->data)->time--;
        currTimeElem = currTimeElem->next;
    }
}

PCB* TimeList::removeFirst() {
    TimePCB* curr = (TimePCB*)timePCBList.removeFirst();
    if(curr == 0)
        return 0;
    PCB* data = curr->pcb;
    delete curr;
    return data;
}

int TimeList::removeFinishedWaiting() {
    Elem* currTimeElem = timePCBList.getFirst();
    Elem* prevTimeElem = 0;
    Elem* deleteTimeElem = 0;
    PCB* toReleasePCB = 0;
    int inc = 0;
    while(currTimeElem != 0) {
        if(((TimePCB*)currTimeElem->data)->time == 0 && ((TimePCB*)currTimeElem->data)->maxTimeToWait != 0) {
            inc++;
            toReleasePCB = ((TimePCB*)currTimeElem->data)->pcb;
            deleteTimeElem = currTimeElem;
            if(currTimeElem == timePCBList.getFirst()) {
                if(timePCBList.getFirst() == timePCBList.getLast())
                    timePCBList.setLast(0);
                currTimeElem = currTimeElem->next;
                timePCBList.setFirst(currTimeElem);
            }
            else if(currTimeElem == timePCBList.getLast()) {
                prevTimeElem->next = 0;
                currTimeElem = 0;
                timePCBList.setLast(prevTimeElem);
            }
            else {
                prevTimeElem->next = currTimeElem->next;
                currTimeElem = currTimeElem->next;
            }
            lockFlag++;
            delete (TimePCB*)deleteTimeElem->data;
            delete deleteTimeElem;
            lockFlag--;
            toReleasePCB->status = READY;
            toReleasePCB->unblockedWithSignal = 0;
            Scheduler::put(toReleasePCB);
        }
        else {
            prevTimeElem = currTimeElem;
            currTimeElem = currTimeElem->next;
        }
    }
    return inc;
}
