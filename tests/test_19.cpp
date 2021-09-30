#include <iostream.h>
#include "thread.h"
#include "timer.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include <stdlib.h>
#include "flags.h"
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

void dumbSleep(int delay) {
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < delay; ++j);
    }
}

PREPAREENTRY(0x9, 1)

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    Event evt(0x9);
    for (unsigned i = 0; i < 64; ++i) {
        syncPrint("Key %d\n", i);
        evt.wait();
    }
    return 0;
}
