#ifndef SORTLIST_H_
#define SORTLIST_H_

#include "list.h"

struct PriorityData {
    void* data;
    int priority;
    PriorityData(void* d, int p) {
        data = d;
        priority = p;
    }
};

class SortList {
public:
    SortList();
    void insert(void* data, int p);
private:
    List sortedList;
};

#endif /* SORTLIST_H_ */