#include <string>
#include "Graph.h"
#include "GraphNode.h"
#include <iostream>
#include "Constants.h"

using namespace std;

Graph::Graph(){
    capacity = 211;
    graph = new GraphNode[capacity];
    occupied = 0;
}

bool Graph::insert(GraphNode node) {
    bool complete = this->insertHelper(node);
    if (complete == true) {
        cout << "The item has been inserted" << endl;
        return true;
    }
    else{
        cout << "Item already present" << endl;
        return false;
    }
}

bool Graph::insertHelper(GraphNode node) {
    //LINEAR PROBING
    int i = 0;
    int index;
    while (graph[((hash(node.getName())) + i) % capacity].getIndex() != EMPTY && graph[(hash(node.getName())) + i].getIndex() != DELETED) {
      //traverse through occupied buckets
      if (graph[((hash(node.getName())) + i) % capacity].getName().compare(node.getName()) == 0)  //found duplicate
	return false;
      if (i == capacity)//completely filled array
	return false;
      i++;
    }//there is either an empty spot or a deleted entry
    if (graph[((hash(node.getName())) + i) % capacity].getIndex() == DELETED) { //if deledted entry found
      index = (hash(node.getName()) + i) % capacity; //remember the position of the first delete
      while (graph[((hash(node.getName())) + i) % capacity].getIndex() != EMPTY && i < capacity) {
	//STOP WHEN FOUND EMPTY SPOT OR WRAP AROUND THE LIST
	if (graph[((hash(node.getName())) + i) % capacity].getName().compare(node.getName()) == 0)
	  //FOUND A DUPLICATE AFTER FIRST DE:ETE
	  return false;
	i++;
      }// no duplicate
      graph[index] = node; //insert to the first deleted entry
    }
    else { //if find an empty spot before a deleted spot
      graph[(hash(node.getName())) + i] = node;
    }
    occupied++;
    return true;
}

int Graph::hash(string str){
  int hash = 0;
  for (int i = 0; i < str.length(); i++){
    hash = (hash * 101 + str[i]) % capacity;
  }
  return hash;
}

GraphNode* Graph::find(string name) {
    int TableIndex = this->findHelper(name);
    if (TableIndex == NOTFOUND) {
      return NULL;
    }
    else {
      return &this->graph[TableIndex];
    }
}

int Graph::findHelper(string name) {
    //LINEAR PROBING
    //return index of what you are looking for
    int i = 0;
    while (graph[(hash(name) + i) % capacity].getName().compare(name) != 0) {
        //STOP WHEN FOUND NAME
        if (graph[(hash(name) + i) % capacity].getIndex() == EMPTY) //find an empty spot
	  return NOTFOUND;
        if (i == this->capacity)
	  //Go through the whole table and can't find the name
	  return NOTFOUND;
        i++;
    }
    return (hash(name) + i) % capacity;
}

bool Graph::deleteData(string name){
    int index;
    index = this->findHelper(name);

    //nothing to delete
    if(index == NOTFOUND){
        return false;
    }

    else{
      LinkedListNode* iter = this->graph[index].getFriends()->getHead();
      //DELETE THIS PERSON FROM OTHERS' FRIENDLISTS
      for(;iter!=NULL;iter=iter->next){
	int i = this->findHelper(iter->name);
	if(i!=NOTFOUND){
	  graph[i].getFriends()->remove(name);
	}
      }
      this->graph[index].deleteNode();
      return true;
    }
}


void Graph::printGraph() {
    for (int i = 0; i < capacity; i++)
        if (graph[i].getIndex() != EMPTY && graph[i].getIndex() != DELETED) {
            cout << "NAME: " << graph[i].getName() << " GRAPH INDEX: " << i << " FILE INDEX: "<< graph[i].getIndex()<< endl;
        }
}

void Graph::printFriendShip(string name){
    GraphNode* result = this->find(name);
    string print="";
    if(result==NULL){
        cout << name <<" is not in the graph"<<endl;
        return;
    }
    print= result->getFriends()->toString();
    if(print=="")
      cout << name <<" has no friends :("<<endl;
    else{
      cout << name <<" has the following friends:"<<endl;
        cout << result->getFriends()->toString()<<endl;
    }
    return;
}

void Graph::addFriendShip(string name1,string name2){
  if(name1==name2){
    cout <<"You are already friend with yourself" << endl;
    return;
  }
  GraphNode* n1 =this->find(name1);
  GraphNode* n2 =this->find(name2);
  if(n1==NULL || n2==NULL){
    cout <<"One (or both) of the 2 people is not in the list" << endl;
    return;
  }
  n1->getFriends()->insert(name2);
  n2->getFriends()->insert(name1);
  cout <<"Successfully add friendship between" << name1 <<" and "<< name2 << endl;
  return;
}


bool Graph::insertFromUsersInput(GraphNode node) {
  bool complete = this->insert(node);
  if(complete){
    LinkedListNode* iter = node.getFriends()->getHead();

    //INSERT THE PERSON AS A FRIEND OF HIS FRIENDS
    for(;iter!=NULL;iter=iter->next){
      int index=this->findHelper(iter->name);
      if(index!=NOTFOUND){
	if(this->graph[index].getFriends()->findNode(node.getName())!=NULL)
	  //The friend already has inserted person as his/her friend
	  continue;
	this->graph[index].getFriends()->insert(node.getName());
      }
    }
    return true;
  }
  return false;
}
