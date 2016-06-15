#include <fstream>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <algorithm>

#include "Constants.h"
#include "Graph.h"
#include "GraphNode.h"
#include "BTree.h"
#include "BTreeNode.h"
#include "LinkedList.h"
#include "LinkedListNode.h"

using namespace std;

vector<string> split(string str, char delimiter)
{
    vector<string> internal;
    stringstream ss(str);       // turn the string into a stream.
    string tok;
    
    while(getline(ss, tok, delimiter))
      {
	internal.push_back(tok);
      }
    return internal;
}

int main()
{
  Graph graph;
  BTree tree;
  
  FILE* f;
  f = fopen("//Users//Vpsastry//Desktop//CS130A Final Project//output.txt", "w+");
  
  ifstream input("//Users//Vpsastry//Desktop//CS130A Final Project//CS130A Final Project//input.txt");
  
  int index = 0;
  
  if(!input) cerr << "File not found" << endl;
  else
    {
      string line;
      while(getline(input, line))
        {
	  
	  vector<string> words = split(line, ',');
	  //  cout << "Insert  "<< words[0] <<" to graph"<<endl;	 
	  GraphNode GNode = GraphNode(words, index);
	  if(graph.insert(GNode)){
	    //cout << "Insert  "<< words[0] <<" to tree"<<endl;  
	    BTreeNode::DataType data = BTreeNode::DataType(words[0], index);
	    tree.insert(data);
	  }
	  else
	    //IF GRAPH INSERT FAIL, THEN THER PERSON ALREADY EXSITS IN THER SYSTEM
	    //AND WE DONT WANT DUPLICATE
	    continue;
	  
	  cout<< "WRITE "<< words[0]<<" ONTO THE OUTPUT FILE" <<endl;
	  fputs(words[0].c_str(), f);
	  fseek(f, 20+53*index, SEEK_SET);
	  fputs(words[1].c_str(), f);
	  fseek(f, 23+53*index, SEEK_SET);
	  fputs(words[2].c_str(), f);
	  fseek(f, 53+53*index, SEEK_SET);
	  index++;
        }
      cout<<"Print the Graph" << endl;
      graph.printGraph();
      cout << "PRINT TREE TEST" << endl;
      tree.printTree(tree.getRoot());
      cout << "PRINT LEAVES TEST" << endl;
      tree.printLeaves(tree.getRoot());
    }
  input.close();
  
  string response;


  //RUN A USER INPUT PROGRAM
  while (true) {
    cout << "You have the following commands: insert, AddFriendship, exactsearch, ListFriendsInfo, RangeQuery, friendship, PrintAll, delete, exit" << endl;
    cin >> response;
    
    //Make resposne lower case so to catch capitalization Error
    std::transform(response.begin(), response.end(), response.begin(), ::tolower);
    
    if(response.compare("insert")==0){
      string line;
      cout << "Please insert your insertion in the following format:\n"
	   <<"name,age,occupation,friends1,friend2,friend3..."<<endl;
      cin>>line;
      vector<string> words = split(line, ',');
      //cout << "Insert  "<< words[0] <<" to graph"<<endl;
      GraphNode GNode = GraphNode(words, index);
      if(graph.insertFromUsersInput(GNode)){
	//cout << "Insert  "<< words[0] <<" to tree"<<endl;
	BTreeNode::DataType data = BTreeNode::DataType(words[0], index);
	tree.insert(data);
      }
      else{
	cout << "The person is already in the system" << endl;
	continue;
      }
      

      cout<< "WRITE "<< words[0]<<" ONTO THE OUTPUT FILE" <<endl;
      fputs(words[0].c_str(), f);
      fseek(f, 20+53*index, SEEK_SET);
      fputs(words[1].c_str(), f);
      fseek(f, 23+53*index, SEEK_SET);
      fputs(words[2].c_str(), f);
      fseek(f, 53+53*index, SEEK_SET);
      index++;
      
      cout<< "Sucessfully inserted"<<endl;
      cout<< "Back to Main Menu"<<endl;
      continue;
    }
    
    if (response.compare("exactsearch") == 0) {
      string name;
      GraphNode* FindReturn;
      cout << "Enter the name you are looking for" <<endl;
      cin >> name;
      FindReturn=graph.find(name);
      if(FindReturn==NULL){
	cout << "The person you are looking for is not within the system."<<endl;
	cout <<"Go back to Main Menu" <<endl;
	continue;
      }
    

	else{
	  int FileIndex = FindReturn->getIndex();
	  LinkedList* friends = FindReturn->getFriends();
	  char Name [20];
	  char Age [3];
	  char Occupation [30];
	  fseek(f,53*FileIndex,SEEK_SET);
	  fgets(Name,21,f);
	  fseek(f,53*FileIndex+20,SEEK_SET);
	  fgets(Age,4,f);
	  fseek(f,53*FileIndex+23,SEEK_SET);
	  fgets(Occupation,31,f);
   	  cout << "The person you are looking for: " << Name << "," << Age << ","<< Occupation <<endl;
	  cout << "Here are their friends: " << FindReturn->getFriends()->toString()<<endl;
	  cout << "Go back to Main Menu" <<endl;
	  continue;
	
    }
      }
    
    if(response =="addfriendship"){
      string name1,name2;
      cout << "Please insert the name of two people who ALREADY in the list"<<endl;
      cin >> name1 >> name2;
      graph.addFriendShip(name1,name2);
      cout << "Go back to Main Menu" <<endl;
      continue;
    }
    
    if(response == "listfriendsinfo"){
      string name;
      cout <<"Please input the name of the person whose friends you are looking for" <<endl;
      cin>>name;
      GraphNode* nodePtr=graph.find(name);

      if(nodePtr==NULL){
	cout << name << " does not exist in data" << endl;
      }
         
      else{
	//START TRAVERSING THROUGH FRIENDS
	LinkedList* list = nodePtr->getFriends();

	if(list->getHead()==NULL){
	  cout << "This person has no friends"<<endl;
	  continue;
	}
	
	cout <<"Information of friends of " << name << endl;
	for(LinkedListNode* ptr=list->getHead();ptr!=NULL;ptr=ptr->next){
	  GraphNode* temp = graph.find(ptr->name);
	  if(temp==NULL){
	    //CANNOT FIND THE FRIEND IN THE GRAPH
	    cout << ptr->name << endl;
	    continue;
	  }
	  //THE FRIEND IS WITHIN THE GRAPH GRAPH
	  int FileIndex = temp->getIndex();
	  char Name [20];
	  char Age [3];
	  char Occupation [30];
	  fseek(f,53*FileIndex,SEEK_SET);
	  fgets(Name,21,f);
	  fseek(f,53*FileIndex+20,SEEK_SET);
       int y = FileIndex;
	  fgets(Age,4,f);
	  fseek(f,53*y+23,SEEK_SET);
	  fgets(Occupation,31,f);
	  cout  << Name << "," << Age << ","<< Occupation <<endl;
	}
	cout << "Successful Search"<<endl;
      }
      cout << "Go back to Main Menu" <<endl;
      continue;   
      }
    
    if (response=="rangequery") {
      string lowbound, upbound;
      cout << "Enter the lower bound and upper bound in the following format:\n"
	   << "lowbound upbound"<<endl;
      cin >> lowbound >> upbound;

      if(lowbound>upbound){
	cout << "Invalid Input: Lower Bound is greater than Upper bound"<< endl;
	continue;
      }
      
      //RETRIEVE INFO
      vector<BTreeNode::DataType>result=tree.RangeQuery(tree.getRoot(),lowbound, upbound);
      if(result.size()==0)
	cout << "Lower bound is too large for the data set" <<endl;

      //PRINT OUT THE RESULT
      for(int i=0;i<result.size();i++){
	int FileIndex=result.at(i).index;
	char Name [20];
	char Age [3];
	char Occupation [30];
	fseek(f,53*FileIndex,SEEK_SET);
	fgets(Name,21,f);
	fseek(f,53*FileIndex+20,SEEK_SET);
	fgets(Age,4,f);
	fseek(f,53*FileIndex+23,SEEK_SET);
	fgets(Occupation,31,f);
	cout  << Name << "," << Age << ","<< Occupation <<endl; 
      }
      cout << "Go back to Main Menu" <<endl;
      continue;
    }

    if (response.compare("friendship") == 0) {
      string name;
      cout << "Enter the name of the person whose friendship you are looking for" << endl;
	cin >> name;
	graph.printFriendShip(name);
	continue;
      }
    
      if(response=="printall"){
	cout << "PRINTING ALL..." <<endl;
	vector<BTreeNode::DataType>result=tree.RangeQuery(tree.getRoot(),"", "zzzzzzzzzzzzzzzzzzzz");
	for(int i=0;i<result.size();i++){
	  int FileIndex=result.at(i).index;
	  
	  char Name [20];
	  char Age [3];
	  char Occupation [30];
	  fseek(f,53*FileIndex,SEEK_SET);
	  fgets(Name,21,f);
	  int y = FileIndex;
	  fseek(f,53*FileIndex+20,SEEK_SET);
	  fgets(Age,4,f);
	  fseek(f,53*y+23,SEEK_SET);
	  fgets(Occupation,31,f);
	  cout  << Name << "," << Age << ","<< Occupation;
       LinkedList *friends = graph.find(result.at(i).name)->getFriends();
       if(friends->toString()!=""){
	    cout<<"," <<friends->toString()<<endl;
	  }
	  else{
	    cout<<endl;
	  }
	}
	cout << "Go back to Main Menu" <<endl;
	continue;
      }
     if(response == "delete"){
          string name;
          cout << "Enter the name of the person to delete"<< endl;
          cin >> name;
	  if (graph.deleteData(name)) {
	    //IF THE PERSON IS WITHIN THE GRAPH
	    cout << name + " has been deleted." << endl;
	    tree.remove(name);
	  }
	  else
	    cout << "Could not delete " + name << endl;
	  cout << "TREE:" << endl;
	  tree.printTree(tree.getRoot());
	  cout << "LEAVES:" << endl;
	  tree.printLeaves(tree.getRoot());
     }
     
     if(response=="printtree"){
       cout << "PRINT TREE TEST" << endl;
       tree.printTree(tree.getRoot());
       continue;
     }
     
     if(response=="printgraph"){
       cout << "PRINT GRAPH ARRAY" << endl;
       graph.printGraph();
       continue;
     }

     if(response=="exit")
       break;

     else{
	cout << "Please only input the suggested commands"<< endl;
	continue;
      }
    }
    fclose(f);
    return 0;

}
