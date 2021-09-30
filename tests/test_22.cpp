#include <iostream.h>
#include "thread.h"
#include "timer.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include <stdlib.h>
#include "flags.h"
#include "usrthr.h"

/**
 * USER9.CPP
 *
 * Creates as many child threads as possible and then exits them all.
 * Child threads are never actually removed from memory, only terminated.
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

class ForkThread : public Thread {
    public:
        ForkThread() : Thread(1048, 1) {}
        virtual void run();
        virtual Thread* clone() const {
            return new ForkThread();
        }
        ~ForkThread() {
            waitToComplete();
        }
        static volatile int failedFork;
};

volatile int ForkThread::failedFork = 0;

void ForkThread::run() {
    while (!failedFork) {
        ID forked = fork();
        if (forked < 0) {
            syncPrint("Failed to fork in thread %d!\n", getRunningId());
            failedFork = 1;
            break;
        } else if (forked == 0) {
            syncPrint("We are in child %d\n", getRunningId());
        } else {
            syncPrint("Cloned thread: %d\n", forked);
            dumbSleep(10000);
        }
    }
    waitForForkChildren();
}

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    ForkThread t;
    t.start();
    return 0;
}
