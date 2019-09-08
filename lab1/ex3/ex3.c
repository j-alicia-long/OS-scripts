/*
Jennifer Long
CS2106
8/9/2019
*/

#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2
#define COLLAPSE 3 // New instruction compared to ex2.

// Node declarations

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
node* findTargetNode(int position, node* originNode);
subNode* findSubNode(int subPosition, node* targetNode);

void insertNodeNext(int,int,node*);
void insertNodePrevious(int,int,node*);
void insertSubNode(int,int,int,node*);
void collapseSubNodes(int, node*);

void deleteAllSubNodes(node*);
void deleteNode(int,node*);
void deleteList(node*);

void printSubNodes(node*);
void printList(node*);


// Start of main
int main()
{
	int position;
	int instruction;
	int subPosition;
	int value;

	// Declaration of the origin Node
	node* originNode = (node*)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;
	originNode->subNodeHead = NULL;


	typedef void (*instr2)(int,node*);
	typedef void (*instr3)(int,int,node*);
	typedef void (*instr4)(int,int,int,node*);

	void (*fptr11)(int,int,node*) = &insertNodePrevious;
	void (*fptr0)(int,node*) = &deleteNode;
	void (*fptr1)(int,int,node*) = &insertNodeNext;
	void (*fptr2)(int,int,int,node*) = &insertSubNode;
	void (*fptr3)(int,node*) = &collapseSubNodes;

	typedef void (*fptr)(void);
	fptr func_table[5] = { (fptr)fptr11, (fptr)fptr0, (fptr)fptr1, (fptr)fptr2, (fptr)fptr3 };

	while(scanf("%i%i",&instruction,&position) == 2)
		//scanf() returns an int that corresponds to the number of values scanned.
	{
		if(instruction == DELETE) // 0
		{
			(*((instr2)func_table[1]))(position,originNode);
			// deleteNode(position,originNode);
		}
		else if(instruction == INSERTSUBNODE) // 2
		{
			scanf("%i%i",&subPosition,&value);
			(*((instr4)func_table[3]))(position,subPosition,value,originNode);
			// insertSubNode(position,subPosition,value,originNode);
		}
		else if(instruction == NEXT) // 1
		{
			scanf("%i",&value);
			(*((instr3)func_table[2]))(position,value,originNode);
			// insertNodeNext(position,value,originNode);
		}
		else if(instruction == PREVIOUS) // -1
		{
			scanf("%i",&value);
			(*((instr3)func_table[0]))(position,value,originNode);
			// insertNodePrevious(position,value,originNode);
		}
		else if(instruction == COLLAPSE) // 3
		{
			(*((instr2)func_table[4]))(position,originNode);
			// collapseSubNodes(position,originNode);
		}
	}
	printList(originNode);
	deleteList(originNode);

	printf("Circular List after delete\n");
	printList(originNode);

	free(originNode);

	return 0;
}


// Function Implementations

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
Updates node data value with sum of sublist values
Then deletes all subNodes
*/
void collapseSubNodes(int position, node* targetNode)
{
	targetNode = findTargetNode(position, targetNode);
	subNode* cur = targetNode->subNodeHead;

	int sum = 0;
	while (cur != NULL) {
		sum += cur->data;
		cur = cur->nextSubNode;
	}

	targetNode->data += sum;
	deleteAllSubNodes(targetNode);
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



//Print list has been implemented for you.
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
