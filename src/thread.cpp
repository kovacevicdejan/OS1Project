#include "thread.h"
#include "timer.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "flags.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock;
	myPCB = new PCB(stackSize, timeSlice, this);
	if(myPCB != 0) {
		if(myPCB->stack == 0) {
		    delete myPCB;
			myPCB = 0;
		}
		else {
			int s = allPCB.insertLast((void*)myPCB);
			if(s == -1) {
				delete myPCB;
				myPCB = 0;
			}
		}
	}
	unlock;
}

void Thread::start() {
	if(myPCB != 0)
	    myPCB->start();
}

void Thread::waitToComplete() {
	if(myPCB == 0)
	    return;
	myPCB->waitToComplete();
}

ID Thread::getId() {
	if(myPCB == 0)
	    return -1;
	return myPCB->getId();
}

ID Thread::getRunningId() {
	return PCB::getRunningId();
}

Thread* Thread::getThreadById(ID id) {
	return PCB::getThreadById(id);
}

ID Thread::fork() {
	lock;
	ID parentID = getRunningId();
	Thread* childThread = running->myThread->clone();

    if(childThread == 0 || childThread->myPCB == 0 || childThread->myPCB->stack == 0) {
		if(childThread != 0)
		    delete childThread;
		unlock;
	    return -1;
    }

    childThread->myPCB->myParent = (PCB*)running;
	int s = running->childrenPCB.insertLast((void*)childThread->myPCB);
	if(s == -1) {
		delete childThread;
		unlock;
		return -1;
	}

	parentPCB = running;
	childPCB = childThread->myPCB;
	unlock;
	PCB::changeChildStack();

	if(parentID == getRunningId()) {
		lock;
		childThread->myPCB->status = READY;
	    Scheduler::put(childThread->myPCB);
        unlock;
		return childThread->getId();
	}
	else if(childThread->getId() == getRunningId()) {
		return 0;
	}
}

void Thread::waitForForkChildren() {
	lock;
    PCB* currPCB = 0;
    while(running->childrenPCB.getFirst() != 0) {
		currPCB = (PCB*)running->childrenPCB.removeFirst();
		if(currPCB == 0)
		    continue;
		unlock;
		currPCB->waitToComplete();
		lock;
	}
	unlock;
}

void Thread::exit() {
	lock;
	PCB* currPCB = 0;

	while(running->waitingPCB.getFirst() != 0) {
		currPCB = (PCB*)running->waitingPCB.removeFirst();
		currPCB->status = READY;
		Scheduler::put((PCB*)currPCB);
	}

    while(running->childrenPCB.getFirst() != 0) {
		currPCB = (PCB*)running->childrenPCB.removeFirst();
		currPCB->myParent = 0;
	}

	running->status = FINISHED;
	unlock;
	dispatch();
}

Thread* Thread::clone() const {
	return new Thread();
}

Thread::~Thread() {
	if(myPCB != 0) {
        lock;
        delete myPCB;
        unlock;
        myPCB = 0;
    }
}

void dispatch() { 
    disable_interrupts;
	context_change_request = 1;
	timer();
	enable_interrupts;
}
