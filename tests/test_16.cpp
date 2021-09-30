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

class EchoThread : public Thread {
    public:
        EchoThread(const char* message, unsigned times=1, unsigned delay=1) :
            Thread(1, 20), message(message), times(times), delay(delay) {}
        virtual void run();
        ~EchoThread() {
            waitToComplete();
        }
    private:
        const char* message;
        unsigned times;
        unsigned delay;
};

void EchoThread::run() {
    syncPrint("First message from thread %d\n", getId());
    for (unsigned i = 0; i < times; ++i) {
        syncPrint("%s from thread %d\n", message, getId());
        dumbSleep(delay * 1000);
    }
    syncPrint("Last message from thread %d\n", getId());
}

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    disable_interrupts;
    EchoThread** threads = new EchoThread*[256];
    enable_interrupts;
    for (unsigned i = 0; i < 256; ++i) {
        disable_interrupts;
        threads[i] = new EchoThread("ECHO", i % 4, i % 10);
        enable_interrupts;
        threads[i]->start();
        if (i % 20 == 0) {
            syncPrint("Dispatching main\n");
            dispatch();
        }
    }
    for (unsigned j = 0; j < 256; ++j) {
        // We cannot wait for a thread to complete while interrupts are locked!
        threads[j]->waitToComplete();
        disable_interrupts;
        delete threads[j];
        enable_interrupts;
    }
    disable_interrupts;
    delete[] threads;
    enable_interrupts;
    return 0;
}
