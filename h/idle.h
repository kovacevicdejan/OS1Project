#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"

class IdleThread : public Thread {
public:
    IdleThread() : Thread(1024, 1) {}

private:
    void run();
};

#endif /* IDLE_H_ */
