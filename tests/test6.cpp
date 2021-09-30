#include <iostream.h>
#include "thread.h"
#include "timer.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
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
 	 Test: idleThread
*/


void tick(){}

class TestThread : public Thread
{
private:
	TestThread **t;

public:

	TestThread(TestThread **thread): Thread(), t(thread){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	(*t)->waitToComplete();
}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread *t1,*t2;
	t1 = new TestThread(&t2);
	t2 = new TestThread(&t1);
	t1->start();
	t2->start();
	delete t1;
	delete t2;
	syncPrintf("Test ends.\n");
	return 0;
}
