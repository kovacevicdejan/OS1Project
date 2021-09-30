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

int syncPrintf(const char *format, ...)
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

Semaphore s(0);
int i = 1;

void tick() {}

class TestThread : public Thread {
public:
    TestThread() : Thread() {}
    ~TestThread() {
        waitToComplete();
    }
protected:
    void run() {
        cout << "Thread " << Thread::getRunningId() << " start!" << endl;
        int res = s.wait((i++) * 30);
        cout << "Thread " << Thread::getRunningId() << " end, r = " << res << endl;
    }
};

int userMain(int argc, char** argv) {
    TestThread tt[5];
    for(int j = 0; j < 5; j++) {
        tt[j].start();
    }
    return 16;
}
