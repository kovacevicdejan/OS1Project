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
#include "list.h"

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

List list;
volatile int listNumbers[256];
volatile unsigned listIndex = 0;

class ListThread : public Thread {
    public:
        ListThread() : Thread(1, rand() % 10) {}
        virtual void run();
};

void ListThread::run() {
    for (unsigned i = 0; i < 4; ++i) {
        disable_interrupts;
        int number = rand();
        int index = listIndex++;
        listNumbers[index] = number;
        syncPrint("Thread %d generated number %d at index %d\n", getId(), number, index);
        enable_interrupts;
        list.insertLast((void*) &listNumbers[index]);
        dumbSleep(number % 1000);
    }
}

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    ListThread thrs[64];
    unsigned i;
    for (i = 0; i < 64; ++i) {
        thrs[i].start();
        if ((i + 1) % 16 == 0) {
            dispatch();
        }
    }
    for (i = 0; i < 64; ++i) {
        thrs[i].waitToComplete();
    }
    syncPrint("THERE SHOULD BE NO THREADS INTERRUPTING\n");
    for (i = 0; i < 256; ++i) {
        int* data = (int*) list.removeFirst();
        if (data == 0) {
            syncPrint("Data missing from index %d!\n", i);
            return 1;
        }
        int number = *data;
        unsigned j = 0;
        for (; j < 256; ++j) {
            if (listNumbers[j] == number) {
                break;
            }
        }
        if (j == 256) {
            syncPrint("Number %d not found!\n", number);
        }
    }
    if (list.removeFirst() != 0) {
        syncPrint("More data in list!\n");
    }
    return 0;
}
