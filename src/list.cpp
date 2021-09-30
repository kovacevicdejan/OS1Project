#include "list.h"
#include "flags.h"

List::List() {
    lock;
    first = 0;
	last = 0;
    n = 0;
    unlock;
}

int List::insertFirst(void* data) {
    lock;
    Elem* newElem = new Elem(data, first);
    if(newElem == 0) {
        unlock;
        return -1;
    }
    if(first == 0)
        last = newElem;
    first = newElem;
    ++n;
    unlock;
    return 0;
}

int List::insertLast(void* data) {
    lock;
    Elem* newElem = new Elem(data, 0);
    if(newElem == 0) {
        unlock;
        return -1;
    }
    if(first == 0)
        first = newElem;
    else
        last->next = newElem;
    last = newElem;
    ++n;
    unlock;
    return 0;
}

void List::removeFromList(void* data) {
    lock;
    Elem* currElem = first;
    Elem* prevElem = 0;
    while(currElem != 0) {
        if(currElem->data == data) {
            if(currElem == first) {
                if(first == last)
                    last = 0;
                first = currElem->next;
            }
            else if(currElem == last) {
                prevElem->next = 0;
                last = prevElem;
            }
            else
                prevElem->next = currElem->next;
            delete currElem;
            --n;
            break;
        }
        prevElem = currElem;
        currElem = currElem->next;
    }
    unlock;
}

void* List::removeFirst() {
    lock;
    if(first == 0) {
        unlock;
        return 0;
    }
    Elem* currElem = first;
    first = first->next;
    if(first == 0)
        last = 0;
    void* currData = currElem->data;
    delete currElem;
    --n;
    unlock;
    return currData;
}

int List::dataInList(void* data) {
    lock;
    Elem* curr = first;
    while(curr != 0) {
        if(curr->data == data) {
            unlock;
            return 1;
        }
        curr = curr->next;
    }
    unlock;
    return 0;
}

int List::length() {
    return n;
}

Elem* List::getFirst() {
    return first;
}

Elem* List::getLast() {
    return last;
}

void List::setFirst(Elem* elem) {
    first = elem;
}

void List::setLast(Elem* elem) {
    last = elem;
}

List::~List() {
    lock;
    Elem* currElem = first;
    Elem* oldElem;
    while(currElem != 0) {
        oldElem = currElem;
        currElem = currElem->next;
        delete oldElem;
    }
    first = last = 0;
    n = 0;
    unlock;
}