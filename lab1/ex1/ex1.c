#include <stdio.h>
#include <stdlib.h> // stdlib includes malloc() and free()
#include <string.h>

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0


// Node declaration ( Do not edit )
typedef struct NODE
{
	int data;
	struct NODE* previousNode;
	struct NODE* nextNode;
} node;

// Function prototypes
void insertNodeNext(int,int,node*);//TODO
void insertNodePrevious(int,int,node*);// TODO
void deleteNode(int,node*); // TODO
void deleteList(node*); // TODO

void printList(node*); // Implemented for you

// Start of main
int main()
{
	int position;
	int instruction;
	int value;

	// Declaration of the origin Node ( do not edit )
	node* originNode = (node*)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;

	// input and processing has been done for you.

	while(scanf("%i%i",&instruction,&position) == 2)
		//scanf() returns an int that corresponds to the number of values scanned.
	{
		if(instruction == DELETE)
		{
			deleteNode(position,originNode);
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
	Inserts a node (with data value) BEFORE the node at position in list originNode
	Allocate memory for new node with malloc()
*/
void insertNodePrevious(int position, int value, node* originNode)
{
	insertNodeNext(--position, value, originNode);
}

/*
	Inserts a node (with data value) AFTER the node at position in list originNode
	Allocate memory for new node with malloc()
*/
void insertNodeNext(int position, int value, node* originNode)
{
	// Allocate memory
	node* newNode = (node*)malloc(sizeof(node)); //do we need casting?
	newNode->data = value;

	// Find prevNode
	node* prevNode = originNode;
	while(position != 0) {
		prevNode = prevNode->nextNode;
		position--;
	}
	// Insert new node
	newNode->nextNode = prevNode->nextNode;
	newNode->previousNode = prevNode;
	newNode->nextNode->previousNode = newNode;
	newNode->previousNode->nextNode = newNode;
}

/*
	Removes a node at position in list originNode
	Deallocate memory using free()
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
  free(node);
}

/*
	Deletes all nodes in list originNode excluding originNode
	Deallocate memory using free()
*/
void deleteList(node* originNode)
{
	node *cur = originNode->nextNode;
  node *temp = malloc(sizeof(cur));
  while (cur != originNode) {
      temp = cur->nextNode;
      free(cur);
      cur = temp;
  }
  originNode->nextNode = originNode;
	originNode->previousNode = originNode;
}

// printList has been implemented for you.
// You may study the functions below to help you implement your own functions.
void printList(node* originNode)
{
	int count = 0;

	printf("Printing clockwise:\n");
	node* iterator = originNode->nextNode;
	printf("[Pos %i:%i]",0,originNode->data);
	printf("\n   |\n   v\n");
	while(iterator != originNode)
	{
		count++;
		printf("[Pos %i:%i]",count,iterator->data);
		printf("\n   |\n   v\n");
		iterator = iterator->nextNode;
	}
	printf("[Pos %i:%i]",0,originNode->data);

	printf("\nPrinting counter-clockwise:\n");
	iterator = originNode->previousNode;
	printf("[Pos %i:%i]",0,originNode->data);
	printf("\n   |\n   v\n");
	while(iterator!= originNode)
	{
		printf("[Pos %i:%i]",count,iterator->data);
		printf("\n   |\n   v\n");
		iterator = iterator->previousNode;
		count--;
	}
	printf("[Pos %i:%i]\n",0,originNode->data);

	return;
}
