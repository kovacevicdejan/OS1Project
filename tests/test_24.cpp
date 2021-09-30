#include <iostream.h>
#include "thread.h"
#include "timer.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include <stdlib.h>
#include "flags.h"
#include "semaphor.h"
#include "usrthr.h"

/**
 * USER11.CPP
 *
 * Producer/consumer example but with fork().
 */

int syncPrint(const char *format, ...)
{
	int res;
	va_list args;
	lock;
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock;
	return res;
}

void dumbSleep(int delay) {
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < delay; ++j);
    }
}

void tick() {}

const unsigned N = 10;
const unsigned M = 10000;
ID threads[N];

Semaphore sem;

void produce() {
    while (1) {
        syncPrint("Prodooc\n");
        sem.signal();
        dumbSleep(rand() % 1000);
    }
}

void consume() {
    while (1) {
        int waitResult = sem.wait(0);
        if (waitResult) {
            syncPrint("Consoomed %d\n", Thread::getRunningId());
        } else {
            syncPrint("CONSOOM FAILED %d\n", Thread::getRunningId());
        }
        dumbSleep(rand() % 1000);
    }
}

class ForkThread : public Thread {
    public:
        ForkThread() : Thread(1, 1) {}
        ~ForkThread() {
            waitToComplete();
        }
        virtual Thread* clone() const {
            return new ForkThread();
        }
        virtual void run();
};

void ForkThread::run() {
    ID lastId;
    for (unsigned i = 0; i < 11; ++i) {
        lastId = fork();
        if (lastId < 0) {
            syncPrint("Failed to fork on index %u!\n", lastId);
            return;
        } else if (lastId > 0) {
            if (i == 0) {
                produce();
            } else {
                consume();
            }
        }
    }
    waitForForkChildren();
}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    ForkThread t;
    t.start();
    return 0;
}
