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
#include "event.h"
#include "ivtentry.h"

/**
 * USER10.CPP
 *
 * Creates as many child threads as possible and then exits them all.
 * Same as USER9.CPP but forks the main thread instead.
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

volatile int failedFork = 0;

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    while (!failedFork) {
        ID forked = Thread::fork();
        if (forked < 0) {
            syncPrint("Failed to fork in thread %d!\n", Thread::getRunningId());
            failedFork = 1;
            break;
        } else if (forked == 0) {
            syncPrint("We are in child %d\n", Thread::getRunningId());
        } else {
            syncPrint("Cloned thread: %d\n", forked);
        }
    }
    Thread::waitForForkChildren();
    return 0;
}
