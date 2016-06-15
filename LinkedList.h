#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <string>
#include "LinkedListNode.h"
#include "Constants.h"

using namespace std;

class LinkedList{
private:
	LinkedListNode *head;
	LinkedListNode *tail;
public:
	LinkedList();
	LinkedList(LinkedList const& orig);
	LinkedListNode* getHead(){return head;}
	void insert(string name);
	bool remove(string name);
	LinkedListNode* findNode(string name);
	LinkedList& operator=(const LinkedList& rhs);
	string toString();
	~LinkedList();
};

#endif //LINKEDLIST_H
