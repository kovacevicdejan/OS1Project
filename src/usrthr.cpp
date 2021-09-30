#include "usrthr.h"

int userMain(int argc, char** argv);

UserThread::UserThread(int argc, char** argv) : Thread() {
    this->argc = argc;
    this->argv = argv;
    res = 0;
}

Thread* UserThread::clone() const {
    return new UserThread(argc, argv);
}

void UserThread::run() {
    res = userMain(argc, argv);
}

int UserThread::getRes() const {
    return res;
}

UserThread::~UserThread() {
    waitToComplete();
}