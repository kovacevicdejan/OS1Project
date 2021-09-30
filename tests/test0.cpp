#include <iostream.h>
#include "thread.h"
#include "timer.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include "flags.h"
#include "usrthr.h"

class TestA : public Thread {
public:
    TestA (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) : Thread(stackSize, timeSlice) {}
    void run();
    ~TestA() { waitToComplete(); }
};

class TestB : public Thread {
public:
    TestB (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) : Thread(stackSize, timeSlice) {}
    void run();
    ~TestB() {waitToComplete();}
};

void TestA::run(){
    for (int i =0; i < 30; ++i) {
        disable_interrupts;
        cout<<"u a() i = "<<i<<endl;
        enable_interrupts;
        for (int k = 0; k<10000; ++k)
            for (int j = 0; j <30000; ++j);
    }
}

void TestB::run(){
    for (int i =0; i < 30; ++i) {
        disable_interrupts;
        cout<<"u b() i = "<<i<<endl;
        enable_interrupts;
        for (int k = 0; k<30000; ++k)
            for (int j = 0; j <30000; ++j);
    }
}

void tick() {}

int userMain(int argc, char* argv[])
{
    disable_interrupts;

    TestA* p1 = new TestA();

    disable_interrupts;
    cout<<"Napravio a"<<endl;
    enable_interrupts;

    TestB* p2 = new TestB();

    disable_interrupts;
    cout<<"Napravio b"<<endl;
    enable_interrupts;

    enable_interrupts;

    p1->start();
    p2->start();
    for (int i = 0; i < 30; ++i) {
        lock;
        cout<<"main "<<i<<endl;
        unlock;

        for (int j = 0; j< 30000; ++j)
            for (int k = 0; k < 30000; ++k);
    }

    disable_interrupts;
    delete p1;
    delete p2;
    enable_interrupts;

    disable_interrupts;
    cout<<"Srecan kraj!"<<endl;
    enable_interrupts;

    return 0;
}

int main(int argc, char** argv) {
    inic();
    //UserThread ut(argc, argv);
	//ut.start();
	//ut.waitToComplete();
    int res = userMain(argc, argv);
    restore();
    return res;
}