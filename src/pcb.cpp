#include "pcb.h"
#include <dos.h>
#include <iostream.h>
#include "SCHEDULE.H"
#include "flags.h"

volatile ID PCB::IDCounter = 0;
volatile PCB mainPCB;
volatile PCB* running = &mainPCB;
volatile List allPCB;
volatile IdleThread idle;
volatile PCB* idlePCB = 0;
volatile PCB* parentPCB = 0;
volatile PCB* childPCB = 0;

const StackSize maxStackSize = 65535;
const StackSize minStackSize = 1024;

unsigned parentSP, parentSS, parentBP;
unsigned previousBP;
unsigned currentBP;
unsigned* pointer;

PCB::PCB(Time timeSlice) {
	lock;
	stack = 0;
	sp = 0;
	bp = 0;
	ss = 0;
	this->timeSlice = timeSlice;
	myThread = 0;
	myParent = 0;
	status = RUNNING;
	unblockedWithSignal = 1;
	id = IDCounter++;
	allPCB.insertLast((void*)this);
	unlock;
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread) {
	lock;
	if(stackSize > maxStackSize)
	    stackSize = maxStackSize;
	if(stackSize < minStackSize)
	    stackSize = minStackSize;
	StackSize size = stackSize / sizeof(unsigned);
	stack = new unsigned[size];

    if(stack != 0) {
		stack[size - 1] = 0x200;
		#ifndef BCC_BLOCK_IGNORE
		stack[size - 2] = FP_SEG(PCB::wrapper);
		stack[size - 3] = FP_OFF(PCB::wrapper);

		sp = FP_OFF(stack + size - 12);
		ss = FP_SEG(stack + size - 12);
		bp = FP_OFF(stack + size - 12);

		stack[size - 12] = 0;
		#endif
	}
    this->timeSlice = timeSlice;
    this->myThread = myThread;
	this->myParent = 0;
	this->status = CREATED;
	this->unblockedWithSignal = 1;
	this->stackSize = size;
	id = IDCounter++;
	unlock;
}

Thread* PCB::getMyThread() {
	return myThread;
}

void PCB::wrapper() {
	running->myThread->run();
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

void PCB::start() {
	lock;
    if(status == CREATED){
		status = READY;
		Scheduler::put(this);
	}
	unlock;
}

void PCB::waitToComplete() {
	if(this != running && status != CREATED && status != FINISHED && status != IDLE) {
		lock;
	    running->status = BLOCKED;
	    waitingPCB.insertFirst((void*)running);
		unlock;
	    dispatch();
	}
}

ID PCB::getId() {
	return id;
}

ID PCB::getRunningId() {
	return ((PCB*)running)->id;
}

Thread* PCB::getThreadById(ID id) {
	lock;
    Elem* curr = allPCB.getFirst();
	while(curr != 0) {
		if(((PCB*)curr->data)->id == id) {
			unlock;
		    return ((PCB*)curr->data)->myThread;
		}
		curr = curr->next;
	}
	unlock;
	return 0;
}

PCB* PCB::getPCBById(ID id) {
	lock;
    Elem* curr = allPCB.getFirst();
	while(curr != 0) {
		if(((PCB*)curr->data)->id == id) {
			unlock;
		    return (PCB*)(curr->data);
		}
		curr = curr->next;
	}
	unlock;
	return 0;
}

void interrupt PCB::changeChildStack() {
	lock;
	#ifndef BCC_BLOCK_IGNORE
    asm {
		mov parentSP, sp;
		mov parentSS, ss;
		mov parentBP, bp;
	}
	#endif

    for(unsigned i = 0; i < parentPCB->stackSize; i++) {
		childPCB->stack[i] = parentPCB->stack[i];
	}

	currentBP = parentBP;
	#ifndef BCC_BLOCK_IGNORE
	while((previousBP = *((unsigned*)MK_FP(parentSS, currentBP))) != 0) {
		pointer = (unsigned*)MK_FP(parentSS, currentBP);
		*(childPCB->stack + (FP_OFF(pointer) - FP_OFF(parentPCB->stack))) = 
		    previousBP + (FP_OFF(childPCB->stack) - FP_OFF(parentPCB->stack));
		currentBP = previousBP;
	}

	childPCB->sp = parentSP + (FP_OFF(childPCB->stack) - FP_OFF(parentPCB->stack));
	childPCB->bp = parentBP + (FP_OFF(childPCB->stack) - FP_OFF(parentPCB->stack));
	childPCB->ss = parentSS + (FP_SEG(childPCB->stack) - FP_SEG(parentPCB->stack));
	unlock;
	#endif
}

PCB::~PCB() {
	lock;
    if(stack != 0){
		delete[] stack;
		stack = 0;
	}

	if(this->myParent != 0) {
	    Elem* curr = this->myParent->childrenPCB.getFirst();
		while(curr != 0) {
			if(((PCB*)curr->data) == this) {
				curr->data = 0;
				break;
			}
		}
	}

	allPCB.removeFromList((void*)this);
	unlock;
}
