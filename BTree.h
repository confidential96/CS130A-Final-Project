#ifndef BTREE_H
#define BTREE_H

#include "BTreeNode.h"
#include "Constants.h"
#include <string>
#include <vector>

using namespace std;

class BTree {
private:
    BTreeNode* root;
    void AdjustingRoot(BTreeNode* leaf);
public:
    BTree();
    void insert(BTreeNode::DataType data);
    void printRange(string lowbound, string upbound);
    void printTree(BTreeNode* root);
    void printLeaves(BTreeNode* root);
    void remove(string name);
    BTreeNode *findNode(BTreeNode *root, string name);
    BTreeNode *getRoot(){return this->root;}
    vector<BTreeNode::DataType> RangeQuery(BTreeNode *root, string lowbound, string upbound);
    string getKey(BTreeNode *root);

};
#endif //BTREE_H
