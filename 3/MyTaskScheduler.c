// Author: Amel Johny
// Student ID: 5294308
// Platform: Windows 10 , GCC compiler 4.8.3

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// This template is only a guide
// You need to include additional fields, data structures and functions

// data type for heap nodes
typedef struct HeapNode
{
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key;	  //key of this task
	int TaskName; //task name
	int Etime;	  //execution time of this task
	int Rtime;	  // release time of this task
	int Dline;	  // deadline of this task
	struct HeapNode *childNode;
	struct HeapNode *sibling;
	int degree;
} HeapNode;

//data type for a priority queue (heap)
typedef struct BinomialHeap
{						//this is heap header
	int size;			// count of items in the heap
	HeapNode *heapHead; // add additional fields here
} BinomialHeap;

// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
	// you need to define other parameters yourself)
	HeapNode *new;
	new = malloc(sizeof(HeapNode));
	assert(new != NULL);
	new->key = k;
	new->TaskName = n;
	new->Etime = c;
	new->Rtime = r;
	new->Dline = d;
	new->sibling = NULL;
	new->childNode = NULL;
	new->degree = 0;
	// initialise other fields
	return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert(T != NULL);
	// initialise all the fields here
	T->heapHead = NULL;
	T->size = 0;
	return T;
}

// Helper Functions
HeapNode *merge(HeapNode *node1, HeapNode *node2);



// put the time complexity analysis for Insert() here
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline
	// You don't need to check if this task already exists in T
	//put your code here

	//k varies to change the  key factor
	//creating a new node
	HeapNode *newNode = newHeapNode(k, n, c, r, d);
	// Set heap head as sibling and change head

	newNode->sibling = T->heapHead;

	T->heapHead = newNode;

	//check everywhere for merges in the heaps of same degree
	HeapNode *prev = NULL;
	HeapNode *current = T->heapHead;
	HeapNode *next = T->heapHead->sibling;
	while ((current != NULL) && (next != NULL))
	{
		// printf("\nheadKey: %d", T->heapHead->key);
		// printf("\nsiblingKey: %d", T->heapHead->sibling->key);
		if (current->degree == next->degree)
		{
			current = merge(current, next);
			if (!prev)
			{
				T->heapHead = current;
			}
			next = current->sibling;
		}
		else
		{
			prev = current;
			current = next;
			next = current->sibling;
		}
	}
	T->size += 1;
}

// put your time complexity for RemoveMin() here
HeapNode *RemoveMin(BinomialHeap *T)
{
	// put your code here
}

int Min(BinomialHeap *T)
{
	// put your code here
}

// put your time complexity analysis for MyTaskScheduler here
int TaskScheduler(char *f1, char *f2, int m)
{
	// put your code here
	int inputCount, name, excecutionT, releaseT, deadline;
	FILE *inputFile;
	inputFile = fopen(f1, "r");
	//TODO: create three heaps for releaseT, deadline and schedule Point
	BinomialHeap *releaseTHeap = newHeap();
	if (inputFile == NULL)
	{ //checks if  file  exists
		exit(0);
	}
	else
	{
		inputCount = fscanf(inputFile, "%d%d%d%d", &name, &excecutionT, &releaseT, &deadline);
		while (inputCount != EOF) //check if input completed
		{
			if (inputCount == 4) // check if there are set of four values for each task
			{					 //TODO: insert to binary heap
				//release time as key
				Insert(releaseTHeap, releaseT, name, excecutionT, releaseT, deadline);
				// printf("%d %d %d %d\n", name, excecutionT, releaseT, deadline);
				inputCount = fscanf(inputFile, "%d%d%d%d", &name, &excecutionT, &releaseT, &deadline);
			}
			else
			{
				printf("input error when reading the attribute of the task %s", f1);
				exit(0);
			}
		}
	}
}

int main() //sample main for testing
{
	int i;
	i = TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
	// if (i == 0)
	// 	printf("No feasible schedule!\n");
	// /* There is a feasible schedule on 4 cores */
	// i = TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
	// if (i == 0)
	// 	printf("No feasible schedule!\n");
	// /* There is no feasible schedule on 3 cores */
	// i = TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
	// if (i == 0)
	// 	printf("No feasible schedule!\n");
	// /* There is a feasible schedule on 5 cores */
	// i = TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
	// if (i == 0)
	// 	printf("No feasible schedule!\n");
	// /* There is no feasible schedule on 4 cores */
	// i = TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
	// if (i == 0)
	// 	printf("No feasible schedule!\n");
	// /* There is no feasible schedule on 2 cores */
	// i = TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
	// if (i == 0)
	// 	printf("No feasible schedule!\n");
	// /* There is a feasible schedule on 2 cores */
	return 0;
}

HeapNode *merge(HeapNode *node1, HeapNode *node2)
{
	if (node1->key < node2->key)
	{
		node1->sibling = node2->sibling;
		node2->sibling = node1->childNode;
		node1->childNode = node2;
		node1->degree++;
		return node1;
	}
	else if ((node1->key == node2->key) && (node1->TaskName < node2->TaskName))
	{
		node1->sibling = node2->sibling;
		node2->sibling = node1->childNode;
		node1->childNode = node2;
		node1->degree++;
		return node1;
	}
	else
	{
		node1->sibling = node2->childNode;
		node2->childNode = node1;
		node2->degree++;
		return node2;
	}
}