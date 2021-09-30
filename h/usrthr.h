#ifndef USRTHR_H_
#define USRTHR_H_

#include "thread.h"

class UserThread : public Thread {
public:
    UserThread(int argc, char** argv);
    int getRes() const;
    Thread* clone() const;
    ~UserThread();

private:
    int argc;
    char** argv;
    int res;

    void run();
};

#endif /* USRTHR_H_ */