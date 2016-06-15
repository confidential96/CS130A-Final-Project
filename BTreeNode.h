#ifndef BTREENODE_H
#define BTREENODE_H
#include <string>
#include "Constants.h"

using namespace std;

class BTreeNode {
public:

  /*
   *DATA MEMBERS
   */
  
  struct DataType {
        string name;
        int index;
        DataType() {
            name = "";
            index = EMPTY;
        }
        DataType(string name, int index) {
            this->name = name;
            this->index = index;
        }
        DataType& operator=(const DataType& rhs) {
            this->name = rhs.name;
            this->index = rhs.index;
            return *this;
        }
    };
  
  //INTEIONALLY MAKE THE ARRAYS LARGER SO WE CAN HAVE EASE MOVING THE INPUT
  DataType* DataArray = new DataType[M];
  BTreeNode** PointerArray = new BTreeNode*[M+1];

  
  int occupied;
  bool isLeaf;
  BTreeNode* Parent;

  //CONSTRUCTORS
  BTreeNode(bool leaf);
  BTreeNode(DataType* entriesArray, int occupied, BTreeNode* Parent);
  BTreeNode(DataType* entriesArray, BTreeNode** PtrArray, int occupied, BTreeNode* Parent);
  
  //DESTRUCTOR
  ~BTreeNode();
  
  //INSERTION FUNCTIONS
    void insertSortLeaf(DataType data);
    void insertSortInternalNode(DataType data, BTreeNode* Child);
    void InsertAdjustLeaf(int index);
    void InsertAdjustInternalNode(int index);

    //REMOVE FUNCTIONS
    BTreeNode* remove(string name);
    void RemoveAtLeaf(int index);
    bool BorrowFromLeavesSibling(BTreeNode *left, BTreeNode*right, int index);
    BTreeNode* MergeLeaves(BTreeNode *left, BTreeNode *right,int index);
    bool BorrowFromInternalNodeSibling(BTreeNode* left, BTreeNode* right,int index);
    BTreeNode* DeleteLeavesAdjustInternalNode(BTreeNode* right,int index);
    BTreeNode* AdjustingInternalNode();
    BTreeNode* MergeInternalNode(BTreeNode* left, BTreeNode* right, int index);

    //HELPER FUNCTIONS
    int FindInParent();    
    int find(string name,bool mode);
    string toString();
    void UpdatingTree(BTreeNode* Parent, string deleted, string replace);
};

#endif /*BTREENODE_H*/
