#include "semaphor.h"
#include "flags.h"
#include "kernsem.h"

Semaphore::Semaphore(int init) {
    lock;
    myImpl = new KernelSem(init);
    unlock;
}

int Semaphore::wait(Time maxTimeToWait) {
    return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal() {
    if(myImpl != 0)
        myImpl->signal();
}

int Semaphore::val() const {
    return myImpl->val();
}

Semaphore::~Semaphore() {
    if(myImpl != 0) {
        lock;
        delete myImpl;
        unlock;
        myImpl = 0;
    }
}