#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include "GraphNode.h"
#include "Constants.h"

using namespace std;

class Graph{
private:
    int capacity;
    int occupied;
    GraphNode* graph;
    int hash(string str);
    bool insertHelper(GraphNode node);
    int findHelper(string name);
public:
    Graph();
    bool insert(GraphNode node);
    GraphNode* find(string name);
    void printGraph();
    void printFriendShip(string name);
    void addFriendShip(string name1,string name2);
    int ExactSearch(string name);
    bool deleteData(string name);
    bool insertFromUsersInput(GraphNode node);
};


#endif //GRAPH_H
