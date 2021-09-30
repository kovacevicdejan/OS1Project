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
#include "kernsem.h"

Semaphore s(0);

void tick() {}

int userMain(int argc, char** argv) {
    while(1) {
        cout << "Number of semaphores: " << allKernelSem.length() << endl;
    }
}