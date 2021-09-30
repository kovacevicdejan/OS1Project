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

class ExitThread : public Thread {
    public:
        ExitThread() : Thread(1, 1) {}
        virtual void run();
        ~ExitThread() {
            waitToComplete();
        }
};

void ExitThread::run() {
    syncPrint("Thread %d sleeping\n", getId());
    dumbSleep(rand() % 1000);
    syncPrint("Thread %d exiting\n", getId());
    exit();
}

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    ExitThread threads[20];
    for (unsigned i = 0; i < 20; ++i) {
        threads[i].start();
    }
    dispatch();
    dispatch();
    dispatch();
    dispatch();
    syncPrint("You've had your chance!\n");
    Thread::exit();
    return 0;
}
