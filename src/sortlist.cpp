#include "sortlist.h"
#include "flags.h"

SortList::SortList() : sortedList() {}

void SortList::insert(void* data, int p) {
    lock;
    Elem* curr = sortedList.getFirst();
    Elem* prev = 0;
    PriorityData* newData = new PriorityData(data, p);
    Elem* newElem = new Elem((void*)newData, 0);

    while(curr != 0 && ((PriorityData*)curr->data)->priority > p) {
        prev = curr;
        curr = curr->next;
    }

    if(prev == 0) {
        newElem->next = sortedList.getFirst();
        sortedList.setFirst(newElem);
    }
    else if(curr == 0) {
        prev->next = newElem;
        sortedList.setLast(newElem);
    }
    else {
        newElem->next = curr;
        prev->next = newElem;
    }
    unlock;
}