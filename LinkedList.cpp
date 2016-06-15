#include "LinkedList.h"
#include "LinkedListNode.h"
#include <string>
#include <iostream>
#include "Constants.h"

using namespace std;

LinkedList::LinkedList(){
    this->head = NULL;
    this->tail = NULL;
}

LinkedList::LinkedList(LinkedList const& orig){
  //COPY CONSTRUCTOR
  this->~LinkedList();
  LinkedListNode *iter;
  iter = orig.head;
  while (iter != NULL) {
    this->insert(iter->name);
    iter = iter->next;
  }
}

void LinkedList::insert(string name){
    
    if (this->head == NULL){
      //EMPTY LINKEDLIST
      head = new LinkedListNode(name);
      tail = head;
    }
    else{
      //INSERT AT TAIL
      tail->next = new LinkedListNode(name);
      tail = tail->next;
    }
}


bool LinkedList::remove(string name) {
  LinkedListNode* iter = head;

  //EMPTY LIST
  if (iter == NULL)
    return false;

  //HEAD CASE
  if (iter->name == name) {
    head = iter->next;
    delete iter;
    if (head == NULL)
      //delete from a list with only one node
      tail = NULL;
    return true;
  }

  //ALL OTHER CASES
  else {
    for (; iter->next != NULL; iter = iter->next) {
      if (iter->next->name == name) {
	LinkedListNode* temp = iter->next;
	iter->next = temp->next;

	//TAIL CASE
	if (tail == temp)
	  tail = iter;
	delete temp;
	return true;
      }
    }
  }
  return false;
}

LinkedListNode* LinkedList::findNode(string name) {
    LinkedListNode* iter = head;
    for (; iter != NULL; iter = iter->next) {
      if (iter->name == name)
	return iter;
    }
    return NULL;
}

LinkedList& LinkedList::operator=(const LinkedList& rhs) {
  this->~LinkedList();
    LinkedListNode *iter;
    iter = rhs.head;
    while (iter != NULL) {
      this->insert(iter->name);
      iter = iter->next;
    }
    return *this;
}


LinkedList::~LinkedList(){
  if (head != NULL) {
    LinkedListNode* delNode;
    while (head != NULL) {
      delNode = head;
      head = head->next;
      
      delete delNode;
    }
    tail = NULL;
  }
}


string LinkedList::toString() {
    string result="";
    if(this->head == NULL){
      return result;
    }
    for (LinkedListNode* iter = this->head; iter != NULL; iter = iter->next) {
        if(iter->name != ""){
            result += iter->name;
        }
        if (iter != tail)
            result += ", ";
        else {
            result += ".";
        }
    }
    return result;
}
