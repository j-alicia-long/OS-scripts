/*
Jennifer Long
CS2106
8/9/2019
*/

#include <stdio.h>
#include <stdlib.h> // stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2 // New instruction compared to ex1.

// Node and SubNode declarations ( Do not Edit )
typedef struct SUBNODE
{
	int data;
	struct SUBNODE* nextSubNode;
} subNode;

typedef struct NODE
{
	int data;
	struct NODE* previousNode;
	struct NODE* nextNode;
	subNode* subNodeHead;
} node;

// Function prototypes
void insertNodeNext(int,int,node*); // TODO
void insertNodePrevious(int,int,node*); //TODO
void deleteNode(int,node*); // TODO
void deleteList(node*); // TODO
void insertSubNode(int,int,int,node*); // TODO
void deleteAllSubNodes(node*); // Optional TODO: will be useful in ex3.

void printList(node*); // Implemented for you
void printSubNodes(node*); // Implemented for you :Helper function for print list

// Start of main
int main()
{
	int position;
	int instruction;
	int subPosition;
	int value;

	// Declaration of the origin Node ( do not edit )
	node* originNode = (node*)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;
	originNode->subNodeHead = NULL;

	// input and processing has been done for you
	while(scanf("%i%i",&instruction,&position) == 2)
		//scanf() returns an int that corresponds to the number of values scanned.
	{
		if(instruction == DELETE)
		{
			deleteNode(position,originNode);
		}
		else if(instruction == INSERTSUBNODE)
		{
			scanf("%i%i",&subPosition,&value);
			insertSubNode(position,subPosition,value,originNode);
		}
		else if(instruction == NEXT)
		{
			scanf("%i",&value);
			insertNodeNext(position,value,originNode);
		}
		else if(instruction == PREVIOUS)
		{
			scanf("%i",&value);
			insertNodePrevious(position,value,originNode);
		}
	}

	printList(originNode);
	deleteList(originNode);

	printf("Circular List after delete\n");
	printList(originNode);

	free(originNode);
	return 0;
}


//Function Implementations

/*
Find position in list originNode and returns targetNode
*/
node* findTargetNode(int position, node* originNode)
{
	node* targetNode = originNode;
	while (position != 0) {
		if (position < 0) {
			targetNode = targetNode->previousNode;
			position++;
		}
		else {
			targetNode = targetNode->nextNode;
			position--;
		}
	}
	return targetNode;
}

/*
Inserts a node (with data value) BEFORE the node at position in list originNode
Allocates memory for new node with malloc()
*/
void insertNodePrevious(int position, int value, node* originNode)
{
	insertNodeNext(--position, value, originNode);
}

/*
Inserts a node (with data value) AFTER the node at position in list originNode
Allocates memory for new node with malloc()
*/
void insertNodeNext(int position, int value, node* originNode)
{
	// Allocate memory
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = value;

	// Find target node position
	node* prevNode = findTargetNode(position, originNode);

	// Insert new node
	newNode->nextNode = prevNode->nextNode;
	newNode->previousNode = prevNode;
	newNode->nextNode->previousNode = newNode;
	newNode->previousNode->nextNode = newNode;
	newNode->subNodeHead = NULL;
}

/*
Find position in sublist of targetNode and return target subnode
*/
subNode* findSubNode(int subPosition, node* targetNode)
{
	subNode* targetSubNode = targetNode->subNodeHead;
	while (subPosition > 0 && targetSubNode != NULL) {
		targetSubNode = targetSubNode->nextSubNode;
		subPosition--;
	}
	return targetSubNode;
}

/*
Inserts a node (with data value) BEFORE the node at subposition in sublist
Allocates memory for new subNode with malloc()
*/
void insertSubNode(int position, int subPosition, int value, node* originNode)
{
	// Allocate memory
	subNode* newSubNode = (subNode*)malloc(sizeof(subNode));
	newSubNode->data = value;

	// Find target node position
	node* targetNode = findTargetNode(position, originNode);

	if (subPosition == 0) { // if head, add subNodeHead
		if (targetNode->subNodeHead != NULL) {
			newSubNode->nextSubNode = targetNode->subNodeHead;
		}
		targetNode->subNodeHead = newSubNode;
	}
	else{ // else, insert in sublist
		// Find subposition
		subNode* prevSubNode = findSubNode(subPosition-1, targetNode);
		// Insert new subnode
		newSubNode->nextSubNode = prevSubNode->nextSubNode;
		prevSubNode->nextSubNode = newSubNode;
	}

}

/*
Deletes all subnodes under given node's subNodeHead
Deallocates memory using free()
A helper function in conjunction with deleteNode().
*/
void deleteAllSubNodes (node* targetNode)
{
	subNode *cur = targetNode->subNodeHead;
	subNode *next;
	while (cur != NULL) {
		next = cur->nextSubNode;
		free(cur);
		cur = next;
	}
	targetNode->subNodeHead = NULL;
}

/*
Removes a node at position in list originNode
Deallocates memory using free()
*/
void deleteNode (int position, node* originNode)
{
	node* node = originNode;
	while(position != 0){
		node = node->nextNode;
		position--;
	}

	node->previousNode->nextNode = node->nextNode;
	node->nextNode->previousNode = node->previousNode;
	deleteAllSubNodes(node);
	free(node);
}

/*
Deletes all nodes in list originNode excluding originNode
Deallocates memory using free()
*/
void deleteList(node* originNode)
{
	// Create temp pointers for deallocation
	node *cur = originNode->nextNode;
	node *next = NULL; // = malloc(sizeof(cur));
	while (cur != originNode) {
		next = cur->nextNode;
		deleteAllSubNodes(cur);
		free(cur);
		cur = next;
	}
	// Reset root node
	originNode->nextNode = originNode;
	originNode->previousNode = originNode;
	deleteAllSubNodes(originNode);
}

// Print list has been implemented for you.
// You may study the functions below to help you implement your own functions.
void printList(node* originNode)
{
	int count = 0;
	printf("Printing clockwise:\n");
	node* iterator = originNode->nextNode;
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while(iterator != originNode)
	{
		count++;
		printf("[Pos %i:%i]",count,iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->nextNode;
	}
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);

	printf("\nPrinting counter-clockwise:\n");
	iterator = originNode->previousNode;
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while(iterator!= originNode)
	{
		printf("[Pos %i:%i]",count,iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->previousNode;
		count--;
	}
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n");

	return;
}

void printSubNodes(node* mainNode)
{
	int count = 0;
	subNode* iterator = mainNode->subNodeHead;
	while(iterator != NULL)
	{
		printf("->[subNode pos %i:%i]",count,iterator->data);
		iterator = iterator->nextSubNode;
		count++;
	}
}
