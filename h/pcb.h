#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "idle.h"
#include "list.h"

enum Status{FINISHED, READY, BLOCKED, CREATED, RUNNING, IDLE};

class PCB {
private:
    volatile static ID IDCounter;
	ID id;
public:
	List waitingPCB;
	List childrenPCB;
	unsigned* stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned timeSlice;
	unsigned stackSize;
	Status status;
	int unblockedWithSignal;
	Thread* myThread;
	PCB* myParent;

    PCB(Time timeSlice = defaultTimeSlice);
	PCB(StackSize stackSize, Time timeSlice, Thread *myThread);
	Thread* getMyThread();
	static void wrapper();
	void start();
    void waitToComplete();
	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);
	static PCB* getPCBById(ID id);
	static void interrupt changeChildStack();
	~PCB();
};

extern volatile PCB mainPCB;
extern volatile PCB* running;
extern volatile List allPCB;
extern volatile IdleThread idle;
extern volatile PCB* idlePCB;
extern volatile PCB* parentPCB;
extern volatile PCB* childPCB;

#endif /* PCB_H_ */
