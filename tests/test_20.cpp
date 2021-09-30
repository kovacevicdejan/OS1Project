#include <iostream.h>
#include "thread.h"
#include "timer.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include <stdlib.h>
#include "flags.h"
#include "usrthr.h"

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

class OveruseThread : public Thread {
    public:
        OveruseThread() : Thread(1, 20) {}
        virtual void run() {
            syncPrint("This should not happen.\n");
        }
        ~OveruseThread() {
            waitToComplete();
        }
};

void tick() {}

Thread* threads[500];

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    unsigned i = 0;
    for (; i < 500; ++i) {
        syncPrint("Creating %d\n", i);
        disable_interrupts;
        threads[i] = new OveruseThread();
        enable_interrupts;
        if (threads[i] == 0 || threads[i]->getId() == -1) {
            syncPrint("Failed at index %d\n", i);
            if (threads[i] != 0) {
                disable_interrupts;
                delete threads[i];
                enable_interrupts;
            }
            break;
        }
    }
    for (unsigned j = 0; j < i; ++j) {
        disable_interrupts;
        delete threads[j];
        enable_interrupts;
    }
    syncPrint("Done\n");
    return 0;
}
