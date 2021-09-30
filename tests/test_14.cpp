#include <iostream.h>
#include "thread.h"
#include "timer.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include <stdlib.h>
#include "flags.h"
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

/*
	Test: brojac sekundi
*/

unsigned t=18;
unsigned seconds = 5;

void secondPast()
{
	if(seconds)
		syncPrintf("%d\n",seconds);
	seconds--;
}

void tick()
{
	t--;
	if(t==0){
		t = 18;
		secondPast();
	}
}

int userMain(int argc, char** argv)
{
	syncPrintf("Starting test\n");
	while(seconds);
	syncPrintf("Test ends\n");
	return 0;
}
