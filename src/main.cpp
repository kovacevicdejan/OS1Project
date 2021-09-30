#include "usrthr.h"
#include "timer.h"

int main(int argc, char** argv) {
    inic();
    UserThread ut(argc, argv);
	ut.start();
	ut.waitToComplete();
    restore();
    return 0;
}