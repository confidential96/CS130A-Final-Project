all: prog3

prog3: Main.o BTree.o Graph.o BTreeNode.o GraphNode.o LinkedList.o LinkedListNode.o 
	g++ -g -o prog3 Main.o BTree.o Graph.o BTreeNode.o GraphNode.o  LinkedList.o LinkedListNode.o

BTree.o: BTree.h BTree.cpp BTreeNode.h
	g++ -c -g BTree.cpp

BTreeNode.o: BTreeNode.h BTreeNode.cpp
	g++ -c -g BTreeNode.cpp

Graph.o: Graph.h Graph.cpp GraphNode.h
	g++ -c -g Graph.cpp

GraphNode.o: GraphNode.h GraphNode.cpp LinkedList.h LinkedList.cpp
	g++ -c -g GraphNode.cpp

LinkedList.o: LinkedList.h LinkedList.cpp LinkedListNode.h
	g++ -c -g LinkedList.cpp

LinkedListNode.o: LinkedListNode.h LinkedListNode.cpp
	g++ -c -g LinkedListNode.cpp

Main.o: Main.cpp Graph.h GraphNode.h BTree.h BTreeNode.h LinkedList.h
	g++ -c -g Main.cpp

clean:
	-rm *.o *~ *# prog3
