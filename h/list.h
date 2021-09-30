#ifndef LIST_H_
#define LIST_H_

#include <iostream.h>

struct Elem {
	void* data;
	Elem* next;
	Elem(void *d, Elem* n) {
		data = d;
		next = n;
	}
};

class List {
private:
	Elem *first, * last;
	int n;
public:
	List();
	int insertFirst(void* data);
	int insertLast(void* data);
	void removeFromList(void* data);
	void* removeFirst();
	int dataInList(void* data);
	int length();
	Elem* getFirst();
	Elem* getLast();
	void setFirst(Elem* elem);
	void setLast(Elem* elem);
	~List();
};

#endif /* LIST_H_ */
