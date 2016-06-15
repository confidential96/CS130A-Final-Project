#include "LinkedListNode.h"
#include <string>
#include "Constants.h"

using namespace std;

LinkedListNode::LinkedListNode(){
	name="";
	next = NULL;
}

LinkedListNode::LinkedListNode(string name){
	this->name=name;
	next = NULL;
}

LinkedListNode& LinkedListNode::operator=(const LinkedListNode& rhs) {
  this->name=rhs.name;
  this->next = rhs.next;
  return *this;
}

LinkedListNode::LinkedListNode(const LinkedListNode& orig) {
  this->name=orig.name;
  this->next = orig.next;
}
