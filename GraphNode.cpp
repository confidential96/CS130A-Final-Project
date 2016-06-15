#include "GraphNode.h"
#include <string>
#include <iostream>
#include <vector>
#include "LinkedList.h"
#include "Constants.h"

using namespace std;

GraphNode::GraphNode(){
    this->name="";
    this->index = EMPTY;
    this->friends = new LinkedList();
}

GraphNode::GraphNode(vector<string> nameArray,int index){
    //cout << "INSERT name into to the node"<<endl;
    this->name=nameArray[0];
    //cout << "INSERT index  into to the node"<<endl;
    this->index = index;
    //cout << "CREATE a new LinkedList of Friendship"<<endl;
    this->friends = new LinkedList();
    for (int i = 3; i < nameArray.size(); i++){
        //cout << "INSERT " << nameArray.at(i)<< " as a friend of " << this->name <<endl;
        this->friends->insert(nameArray.at(i));
    }
}

GraphNode& GraphNode::operator=(const GraphNode& rhs) {
    this->name=rhs.name;
    this->index = rhs.index;
    this->friends = rhs.friends;
    return *this;
}

bool GraphNode::deleteNode(){
    this->name = "";
    this->index=DELETED;
    delete friends;
    return true;
}
