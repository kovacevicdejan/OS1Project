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

void tick() {}

int userMain(int argc, char* argv[]) {
    int failed = 0;
    while(failed == 0){
        int pid = Thread::fork();
        if (pid == -1) {
            failed = 1;
            cout << "Failed fork at " << Thread::getRunningId() << endl;
            break;
        }
        if(pid != 0) 
            break;
        syncPrint("Test %d\n", Thread::getRunningId());
    }
    
    Thread::waitForForkChildren();
    if(Thread::getRunningId() == 3)
        cout << "Happy end!!!" << endl;
}