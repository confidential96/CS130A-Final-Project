//
//  Btree.cpp
//  CS130A Final Project
//
//  Created by Vikram Sastry on 3/6/16.
//  Copyright © 2016 Vikram Sastry and Thien Hoang. All rights reserved.
//
#include "BTree.h"
#include "Constants.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

BTree::BTree(){
  //Default Constructor
  root = new BTreeNode(LEAF);
}

void BTree::insert(BTreeNode::DataType data){
  //ALWAYS INSERT AT LEAF
  BTreeNode* leaf = this->findNode(root, data.name);
  leaf->insertSortLeaf(data);
  this->AdjustingRoot(this->root);
}

void BTree::AdjustingRoot(BTreeNode* leaf) {
  //ADJUSTING ROOT SO IT POINTS AT THE RIGHT PLACE
  if (leaf->Parent == NULL){
    this->root = leaf;
  }
  else {
    this->AdjustingRoot(leaf->Parent);
  }
}

BTreeNode *BTree::findNode(BTreeNode *root, string name) {
  //FIND THE LEAVE CONTAINING NAME
  if (root == NULL) {
        cout <<"EMPTY TREE" <<endl;
        root = new BTreeNode(LEAF);
        return root;
    }
    if (root->isLeaf) {
        return root;
    }
    if (!root->isLeaf) {//looking at the root or the internal node
        for (int i = 0; i <= root->occupied; i++) {
            if (root->DataArray[i].index == EMPTY || name < root->DataArray[i].name) {
                //If we go pass the last entry or find an entry larger than what we're looking for
                return findNode(root->PointerArray[i], name);
            }
        }
    }
    return NULL;
}


void BTree::printRange(string lowbound, string upbound) {
  vector<BTreeNode::DataType> result = this->RangeQuery(this->root, lowbound, upbound);
  for (int i = 0; i < result.size(); i++)
    cout << "Name: " << result.at(i).name << " Index: " << result.at(i).index<< endl;
}

void BTree::printTree(BTreeNode *root) {
    cout << root->toString() << endl;
    if (root->isLeaf)
        return;
    else {
        for (int i = 0; i <= root->occupied; i++) {
            printTree(root->PointerArray[i]);
        }
        return;
    }
}

void BTree::printLeaves(BTreeNode* root) {
    if(!root->isLeaf)
        printLeaves(root->PointerArray[0]);
    else {
        cout << root->toString() << endl;
        if (root->PointerArray[M] != NULL)
            printLeaves(root->PointerArray[M]);
        return;
    }
    return;
}

vector<BTreeNode::DataType> BTree::RangeQuery(BTreeNode *root, string lowbound, string upbound) {
    /*We will find the lower bound in the leaf and start collecting Data from one leaf to
     another until we hit the upper bound*/
  vector<BTreeNode::DataType> result;
  if (root == NULL) {
    cout << "We can't find it" << endl;
    return result;
  }
    if (!root->isLeaf) {// looking at root or internal
      for (int i = 0; i <= root->occupied; i++) {
	if (root->DataArray[i].index == EMPTY || lowbound < root->DataArray[i].name) {
	  //If we go pass the last entry or find an entry larger than the lower bound
	  return RangeQuery(root->PointerArray[i], lowbound, upbound);
	}
      }
    }
    if (root->isLeaf) {
      int j;
      
      for (j = 0; j <= root->occupied; j++) {
            if (root->DataArray[j].index == EMPTY || lowbound.compare(root->DataArray[j].name) <= 0) {
	      //We simply find the element larger than the lowerbound
	      break;
            }
      }
        while (root->DataArray[j].name <= upbound) { // while we haven't exceeded the upperbound
	  if (root->DataArray[j].index != EMPTY){
	    result.push_back(root->DataArray[j]);
	    //cout << root->DataArray[j].name << ", ";
	  }
            if (j >= root->occupied - 1) {//last node or empty node
	      if (root->PointerArray[M] != NULL) {//as long as next leaf is not NULL
		root = root->PointerArray[M];
		j = 0;
		continue;
                }
	      else {//if next leaf is NULL
                    //cout << "Do not exceed upperbound" << endl;
		return result;
	      }
            }
            j++;
        }
        cout <<endl;
    }
    return result;
}

void BTree::remove(string name) {
  //DELETE AT LEAF
  BTreeNode* leaf = findNode(root, name);
  if(leaf->find(name,EXACT)==NOTFOUND){
    cout << "TARGET NOT FOUND" << endl;
    return;
  }
  cout << "Found TARGET. Now deleting it. "<< endl;
  leaf=leaf->remove(name);
  cout << "DELETE SUCESSFULLY. Now ADJUSTING ROOT" <<endl;
  AdjustingRoot(leaf); 
}
