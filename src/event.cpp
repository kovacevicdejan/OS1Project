#include "event.h"
#include "flags.h"
#include "kernev.h"

Event::Event(IVTNo ivtNo) {
    lock;
    myImpl = new KernelEv(ivtNo);
    unlock;
}

void Event::wait() {
    if(myImpl != 0)
        myImpl->wait();
}

void Event::signal() {
    if(myImpl != 0)
        myImpl->signal();
}

Event::~Event() {
    if(myImpl != 0) {
        lock;
        delete myImpl;
        unlock;
    }
    myImpl = 0;
}