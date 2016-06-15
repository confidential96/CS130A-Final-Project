#include "BTreeNode.h"
#include <iostream>
#include <string>
#include "Constants.h"
#include <stdlib.h>
#include <math.h>

using namespace std;

/*
 * CONSTRUTORS AND DESTRUCTOR
 */


BTreeNode::BTreeNode(bool leaf) {
    //USED TO EITHER CREATE AN EMPTY LEAF OR EMPTY INTERNAL NODE
    occupied = 0;
    isLeaf = leaf;
    Parent = NULL;
    for (int i = 0; i <= M; i++)
        this->PointerArray[i] = NULL;
}


BTreeNode::BTreeNode(DataType* entriesArray, int occupied, BTreeNode* Parent) {
    //CONSTRUCTOR FOR NEW LEAVES WHEN SPLITTING
    this->occupied = occupied;
    this->Parent = Parent;
    this->isLeaf = LEAF;
    for (int i = 0; i < occupied; i++)
        this->DataArray[i] = entriesArray[i];
    for (int i = 0; i <= M; i++)
        this->PointerArray[i] = NULL;
}


BTreeNode::BTreeNode(DataType* entriesArray, BTreeNode** PtrArray, int occupied, BTreeNode* Parent) {
    //CONSTRUCTOR FOR NEW INTERNAL NODE WHEN SPLITTING
    this->occupied = occupied;
    this->Parent = Parent;
    this->isLeaf = NOTLEAF;
    for (int i = 0; i <= occupied; i++) {
        if (i<occupied)
            this->DataArray[i] = entriesArray[i];
        this->PointerArray[i] = PtrArray[i];
        this->PointerArray[i]->Parent=this;
    }
}

BTreeNode::~BTreeNode() {
  //DESTRUCTOR

  //cout<< "DELETING BTREENODE" << endl;
  delete[] DataArray;
  for (int i = 0; i <= occupied; i++)
    PointerArray[i] = NULL;
}


/*
 * FUNCTIONS FOR INSERTS
 */


void BTreeNode::insertSortLeaf(DataType data) {
  //INSERT AT LEAF
  int index = find(data.name,BIGGER);
  InsertAdjustLeaf(index);
  this->DataArray[index] = data;
  this->occupied++;
  
  if (occupied > L) {
    //break the L limit
    if (this->Parent == NULL) {
      this->Parent = new BTreeNode(NOTLEAF);
      this->Parent->PointerArray[0] = this;
    }
    
    DataType* CutOffEntries = new DataType[2];
    CutOffEntries[0] = this->DataArray[2];
    CutOffEntries[1] = this->DataArray[3];
    BTreeNode* NewLeaf = new BTreeNode(CutOffEntries, 2, this->Parent);
        //create a new BTree Leaf contains the last two entries of current leaf
        //                    and has its parent
    this->DataArray[2].index = EMPTY; //restore order
    this->DataArray[3].index = EMPTY; //to below L limit
    this->occupied = 2; //because we just cut our leaf in half
    //Insert into the parent Node
    Parent->insertSortInternalNode(CutOffEntries[0], NewLeaf); //RECURSIVE CALL TO INSERT OF THE PARENT
    NewLeaf->PointerArray[M] = this->PointerArray[M];
    this->PointerArray[M] = NewLeaf;
  }
}

void BTreeNode::insertSortInternalNode(DataType data, BTreeNode* Child) {
  //INSERT AT INTERNAL NODE
  int index = find(data.name,BIGGER);
  InsertAdjustInternalNode(index);
  this->DataArray[index] = data;
  this->occupied++;
  this->PointerArray[index + 1] = Child;
  if (occupied > (M - 1)) { // break the M limit
    if (this->Parent == NULL) {
      this->Parent = new BTreeNode(NOTLEAF);
      this->Parent->PointerArray[0] = this;
    }
    
    //Creating new array of data entries
    DataType* CutOffEntries = new DataType[2];
    CutOffEntries[0] = this->DataArray[3];
    CutOffEntries[1] = this->DataArray[4];
    
    //Setting all the cut off entries to EMPTY
    this->DataArray[3].index = EMPTY;
    this->DataArray[4].index = EMPTY;
    
    //Creating new array of pointer
    BTreeNode** CutOffPtr = new BTreeNode*[3];
    CutOffPtr[0] = this->PointerArray[3];
    CutOffPtr[1] = this->PointerArray[4];
    CutOffPtr[2] = this->PointerArray[5];
    
    //Setting all the ptr got cut off to NULL
    this->PointerArray[3] = NULL;
    this->PointerArray[4] = NULL;
    this->PointerArray[5] = NULL;
    
    //Create a new node from the cut off ptr and data
    BTreeNode* NewInternalNode = new BTreeNode(CutOffEntries, CutOffPtr, 2, this->Parent);
    
    //Take the middle node and insert to the parent and point the parent to new Node
    this->Parent->insertSortInternalNode(DataArray[2], NewInternalNode);
	
    this->DataArray[2].index = EMPTY; //because we have moved it up
    this->occupied = 2; //because we have deleted
  }
}

void BTreeNode::InsertAdjustLeaf(int index) {
    if (index == this->occupied)
        return;
    else {
        for (int i = occupied; i > index; i--)
            this->DataArray[i] = this->DataArray[i - 1];
    }
}

void BTreeNode::InsertAdjustInternalNode(int index) {
    if (index == this->occupied)
        return;
    else {
        for (int i = occupied; i > index; i--) {
            this->DataArray[i] = this->DataArray[i - 1];
            this->PointerArray[i + 1] = this->PointerArray[i];
        }
    }
}

/*
 * FUNCTIONS FOR DELETE
 */


BTreeNode* BTreeNode::remove(string name) {
  if (!isLeaf) {//Not Leaf
    cout << "Trying to delete at Internal Node" << endl;
    return this;
  }
  int index = find(name, EXACT); //find the exact spot where the target exist
    if (index == NOTFOUND) {
      cout << "The person you are trying to delete is not within the system"<< endl;
      return this;
    }
    cout << "Remove at Leaf"<<endl;
    RemoveAtLeaf(index); //remove that target
    if (occupied < ceil((double)L / 2) && Parent != NULL) //not within limit
      {
        int index = FindInParent(); //find the index within the parent Node
        bool complete;
        if (index == 0){ //if we are at the leftmost leaf
	  //cout << "TRYING TO BORROW FROM RIGHT LEAF"<< endl;
	  complete = BorrowFromLeavesSibling(this, this->Parent->PointerArray[index + 1], index);
        }else{
	  //cout << "TRYING TO BORROW FROM LEFT LEAF"<< endl;
	  complete = BorrowFromLeavesSibling(this->Parent->PointerArray[index - 1],this, index);
        }
        if (complete){ //swap and problem solved
	  //cout << "Problem Solved" <<endl;
	  this->UpdatingTree(this->Parent,name,this->DataArray[0].name);
	  return this;
        }
        else { //Now we have to merge leaves
	  BTreeNode* MergedLeaf;
	  if (index == 0) {
	    //cout << "TRYING TO MERGE LEAF (SPECIAL CASE)"<< endl;
	    MergedLeaf=MergeLeaves(this, Parent->PointerArray[index + 1],index);
            }
	  else{
	    //cout << "TRYING TO MERGE LEAF (REGULAR CASE)"<< endl;
	    MergedLeaf=MergeLeaves(Parent->PointerArray[index - 1], this,index);
	  }
	  MergedLeaf->UpdatingTree(MergedLeaf->Parent,name,MergedLeaf->DataArray[0].name);
	  return MergedLeaf;
        }
    }
    this->UpdatingTree(this->Parent,name,this->DataArray[0].name);
    return this;
}

BTreeNode* BTreeNode::MergeLeaves(BTreeNode *left, BTreeNode *right, int index) {
    //Always Merge To The LEFT
    cout << "In MergeLeaves"<< endl;
    for (int i = 0; i < right->occupied; i++)
        left->insertSortLeaf(right->DataArray[i]); //insert each data on the right to the left
    left->PointerArray[M] = right->PointerArray[M]; //Make sure the left is pointing to the next leaf
    left->Parent=left->Parent->DeleteLeavesAdjustInternalNode(right,index);
    return left;
}


void BTreeNode::RemoveAtLeaf(int index){
    for (int i = index; i < occupied - 1; i++)
        DataArray[i] = DataArray[i + 1];
    DataArray[occupied-1].index=EMPTY;
    this->occupied--;
    if (this->Parent != NULL && index == 0) {
      for (int i = 0; i <= this->Parent->occupied; i++) {
	if (Parent->PointerArray[i] == this && i!=0)
	  //Parent pointer that is pointing to the node and is not the leftmost
	  Parent->DataArray[i-1]=DataArray[0]; //update parent data array
      }
    }
}

bool BTreeNode::BorrowFromLeavesSibling(BTreeNode *left, BTreeNode*right, int index) {
  //Always Borrow from the LEFT
  
  if (index == 0) {
    //We are at the leftmost leaf, then check the RIGHT sibling
    if (right->occupied <= ceil((double)L / 2))
      return false;
    //ENOUGH DATA TO BORROW
    left->insertSortLeaf(right->DataArray[0]);
    
    right->remove(left->DataArray[left->occupied - 1].name);
        //will not be infinite loop because it has more than L at least by 1
    
    return true;
  }
  else {
    //We are not at the leftmost leaf, so we can check the LEFT sibling
    if (left->occupied <= ceil((double)L / 2))
      return false;
    //ENOUGH DATA TO BORROW
    right->insertSortLeaf(left->DataArray[left->occupied - 1]);
        
    left ->remove(right->DataArray[0].name);
    right->Parent->DataArray[index-1]=right->DataArray[0];
    //will not be infinite loop because it has more than L at least by 1
    
    return true;
    }
}

BTreeNode* BTreeNode::DeleteLeavesAdjustInternalNode(BTreeNode* right,int index) {
    //This is for the parent of the leaf
  cout << "IN DeleteLeavesAdjustInternalNode"<< endl;
    if (index == 0)//Special Case of leftmost leaf
        index = 1;
    for (int j = index; j < this->occupied; j++) {
        //PULLING POINTER AND DATA TO THE LEFT
        this->DataArray[j-1] = this->DataArray[j];
        this->PointerArray[j] = this->PointerArray[j+1];
    }
    this->DataArray[occupied - 1].index = EMPTY;
    this->PointerArray[occupied] = NULL;
    this->occupied--;
    delete right;
    if(this->occupied == 0){
        delete this;
        return NULL;
    }
    if (this->occupied < ceil((double)(M-1) / 2) && this->Parent != NULL) //not within limit
        return this->AdjustingInternalNode();//Design to fix this node
    return this;
}

BTreeNode* BTreeNode::AdjustingInternalNode() {
  //ONLY GET CALLED WHEN THE INTERNAL NODE IS UNDER LOWER BOUND
  int index = this->FindInParent();
  bool complete;
    if (index == 0) //leftmost internal node case
        complete = BorrowFromInternalNodeSibling(this, Parent->PointerArray[index + 1], index);
    else
        complete = BorrowFromInternalNodeSibling(Parent->PointerArray[index - 1], this, index);
    if (complete == true) //task finished
        return this;
    else { //Now we have to merge
        BTreeNode* MergedNode;
        if (index == 0) //leftmost internal node case
            MergedNode=MergeInternalNode(this, Parent->PointerArray[index + 1], index);
        else
            MergedNode=MergeInternalNode(Parent->PointerArray[index - 1], this, index-1);
        return MergedNode;
    }
    return this;
}

bool BTreeNode::BorrowFromInternalNodeSibling(BTreeNode* left, BTreeNode* right,int index) {
    //ALWAYS BORROW FROM LEFT
    cout << "IN BorrowFromInternalNodeSibling" <<endl;
    if (index == 0) { //leftmost internal node case
        if (right->occupied <= ceil((double)(M-1) / 2)) //don't have enough to borrow
            return false;
        else {  //have enough to borrow
            //From Left Pull Down the Parent
            right->PointerArray[0]->Parent = left;
            left->insertSortInternalNode(left->Parent->DataArray[index], right->PointerArray[0]);
            
            //From parent pull Right up
            left->Parent->DataArray[index] = right->DataArray[0];
            
            //PULLING POINTER AND DATA TO THE LEFT ON THE RIGHT NODE
            right->PointerArray[0] = right->PointerArray[1];
            for (int j = 0; j < (right->occupied - 1); j++) {
                right->DataArray[j] = right->DataArray[j + 1];
                right->PointerArray[j+1] = right->PointerArray[j+2];
            }
            right->DataArray[right->occupied - 1].index = EMPTY;
            right->PointerArray[right->occupied] = NULL;
            right->occupied--;
            return true;
        }
    }
    if (left->occupied <= ceil((double)(M-1) / 2))
        return false;
    
    //FROM RIGHT PULL PARENT DOWN
    left->PointerArray[left->occupied]->Parent = right;
    right->insertSortInternalNode(right->Parent->DataArray[index-1], right->PointerArray[0]);
    right->PointerArray[0]=left->PointerArray[left->occupied];
    
    
    //FROM PARENT PULL LEFT UP
    right->Parent->DataArray[index-1] = left->DataArray[left->occupied-1];
    
    //Fix left
    left->DataArray[left->occupied-1].index=EMPTY;
    left->PointerArray[left->occupied]=NULL;
    left->occupied--;
    return true;
}

BTreeNode* BTreeNode::MergeInternalNode(BTreeNode* left, BTreeNode* right, int index) {
    cout << "IN MergeInternalNode"<< endl;
    
    //FROM LEFT, PULL PARENT DOWN
    left->Parent->DataArray[index]=right->PointerArray[0]->DataArray[0];
    right->PointerArray[0]->Parent = left;
    left->insertSortInternalNode(left->Parent->DataArray[index], right->PointerArray[0]);
    
    //merge the RIGHT to LEFT
    for (int i = 0; i < right->occupied; i++) {
        right->PointerArray[i + 1]->Parent=left; //make them all point to left as parent
        left->insertSortInternalNode(right->DataArray[i], right->PointerArray[i + 1]);
    }
    
    //Adjust Parent's Pointer, Data Array
    for (int i = index; i < left->Parent->occupied - 1; i++) {
        left->Parent->DataArray[i] = left->Parent->DataArray[i + 1];
        left->Parent->PointerArray[i + 1] = left->Parent->PointerArray[i + 2];
    }
    delete right;
    left->Parent->DataArray[left->Parent->occupied-1].index = EMPTY;
    left->Parent->PointerArray[left->Parent->occupied] = NULL;
    left->Parent->occupied--;
    if (left->Parent->occupied == 0){//empty parent 
        cout << "DELETING PARENT "<<endl;
        delete Parent;
        left->Parent = NULL;
        return left;
    }
    
    //if Parrent is under bound and is not root, then we recursively call upward
    if (left->Parent->occupied < ceil((double)(M-1) / 2)&&left->Parent->Parent!=NULL) {
        left->Parent=left->Parent->AdjustingInternalNode();
    }
    return left;
}


/*
 * HELPER FUNCTIONS FOR CODE REUSE
 */


int BTreeNode::FindInParent() {
  //FIND THE INDEX IN THE POINTER ARRAY THAT IS POINTEING TO THIS
  for (int i = 0; i <= this->Parent->occupied; i++){
    if (this->Parent->PointerArray[i] == this)
      return i;
  }
    return 0;
}

void BTreeNode::UpdatingTree(BTreeNode* Parent, string deleted, string replace){
  //HELPER FUNCTIONS USED TO UPDATE THE TREE AFTER DELETE
  if(Parent==NULL)
    return;
  for(int i=0; i< Parent->occupied; i++){
    if(Parent->DataArray[i].name==deleted){
      Parent->DataArray[i].name=replace;
      return;
    }
  }
  return UpdatingTree(Parent->Parent,deleted,replace);
}

string BTreeNode::toString() {
  string result = "";
  if (this->isLeaf) {
    result += "LEAF: \n";
    for (int i = 0; i < occupied; i++) {
      result += "    " + this->DataArray[i].name;
    }
  }
  else {
    result += "INTERNAL NODE: \n";
        for (int i = 0; i < occupied; i++) {
	  result += "    " + this->DataArray[i].name;
        }
  }
  return result;
}

int BTreeNode::find(string name, bool mode) {
    if (mode == BIGGER) {
        for (int i = 0; i <= this->occupied; i++) {
            if ((this->DataArray[i].index == EMPTY) || (this->DataArray[i].name > name))
                return i;
        }
    }
    if (mode == EXACT) {
        for (int i = 0; i < this->occupied; i++) {
            if (this->DataArray[i].name == name)
                return i;
        }
        return NOTFOUND;
    }
    return NOTFOUND;
}


