#ifndef GRAPHNODE_H
#define GRAPHNODE_H
#include <string>
#include <vector>
#include "LinkedList.h"
#include "Constants.h"

using namespace std;

class GraphNode{
private:
    string name;
    int index;
    LinkedList* friends;
public:
    GraphNode();
    GraphNode(vector<string> nameArray, int index);
    string getName() { return name; }
    int getIndex() { return index; }
    LinkedList *getFriends() { return friends; }
    GraphNode& operator=(const GraphNode& rhs);
    bool deleteNode();
};

#endif //GRAPHNODE_H
