#ifndef LINKEDLISTNODE_H
#define LINKEDLISTNODE_H
#include <string>
#include "Constants.h"

using namespace std;

class LinkedListNode{
 public:
	string name;
	LinkedListNode *next;
	LinkedListNode();
	LinkedListNode(string name);
	LinkedListNode& operator=(const LinkedListNode& rhs);
	LinkedListNode(const LinkedListNode& orig);
};

#endif //LINKEDLISTNODE_H
