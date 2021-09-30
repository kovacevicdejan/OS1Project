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

Semaphore sem;

class Producer : public Thread {
    public:
        Producer() : Thread(1, 10) {}
        virtual void run();
        ~Producer() {
            waitToComplete();
        }
};

void Producer::run() {
    while (1) {
        syncPrint("Prodooc\n");
        sem.signal();
        dumbSleep(rand() % 1000);
    }
}

class Consumer : public Thread {
    public:
        Consumer() : Thread(1, 10) {}
        virtual void run();
        ~Consumer() {
            waitToComplete();
        }
};

void Consumer::run() {
    while (1) {
        int waitResult = sem.wait(0);
        if (waitResult) {
            syncPrint("Consoomed %d\n", getId());
        } else {
            syncPrint("CONSOOM FAILED %d\n", getId());
        }
        dumbSleep(rand() % 1000);
    }
}

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    Producer p;
    Consumer c[10];
    lock;
    p.start();
    for (unsigned i = 0; i < 10; ++i) {
        c[i].start();
    }
    unlock;
    return 0;
}
