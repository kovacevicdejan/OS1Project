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

void tick() {}

void writer(char* c, int* flag) {
    while (1) {
        while (*flag == 1);
        cin >> (*c);
        *flag = 1;
    }
}

void reader(char* c, int* flag) {
    while (1) {
        while (*flag == 0);
        cout << (*c);
        *flag = 0;
    }
}

void pipe() {
    static char c;
    static int flag = 0;
    if (Thread::fork()) {
        writer(&c,&flag);
    } else {
        reader(&c,&flag);
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
        virtual void run() {
            pipe();
        }
};

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    ForkThread t;
    t.start();
    return 0;
}
