#include <string>
#include "place.h"

class Link {
public:
	place* data;
	Link* next;
	Link(place* loc, Link* link = NULL) {
		data = loc;
		next = link;
	}
	void print() {
		data->print();
	}
};

class List {
protected:
	Link* first;
	Link* last;
	int length;
public:
	List() {
		first = NULL;
		last = NULL;
		length = 0;
	}
	Link* getFirst() { 
		return first; 
	}	

	void addFront(place* loc) {
		first = new Link(loc, first);
		if (last == NULL) last = first;
		length++;
	}
	void addEnd(place* loc) {
		Link* link = new Link(loc, NULL);
		if (last != NULL) 
			last->next = link;
		else first = link;
		last = link;
		length++;
	}
};







