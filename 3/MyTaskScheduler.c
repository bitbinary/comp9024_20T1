// Author: Amel Johny
// Student ID: 5294308
// Platform: Windows 10 , GCC compiler 4.8.3

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct HeapNode
{
	int key;
	int TaskName;
	int Etime;
	int Rtime;
	int Dline;
	struct HeapNode *childNode;
	struct HeapNode *sibling;
	int degree;
} HeapNode;

//data type for a priority queue (heap)
typedef struct BinomialHeap
{
	int size;
	HeapNode *heapHead;
} BinomialHeap;

// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{
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
	return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert(T != NULL);
	T->heapHead = NULL;
	T->size = 0;
	return T;
}

// Helper Functions
HeapNode *minHeapMerge(HeapNode *node1, HeapNode *node2);
HeapNode *revertHeap(HeapNode *start);
HeapNode *heapUnion(HeapNode *node1, HeapNode *node2);

// The time complexity analysis for Insert:
//the insert function has complexity of the loop which merges nodes with same degree.
//Its complexity is O(logn) which inturn makes the complexity of insert: O(logn)
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{
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
			current = minHeapMerge(current, next);
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

// Time complexity for RemoveMin(): Remove consist of functions that
//find the minimum,revvert the heap and heap union.
//each has Time complexity O(logn) and processed one after another.
//therefore the complexity is : O(logn)
HeapNode *RemoveMin(BinomialHeap *T)
{

	if (T->heapHead != NULL)
	{
		HeapNode *currentNode = T->heapHead;
		HeapNode *currentMinimumNode = currentNode;
		HeapNode *currentMinimumNodePrev = NULL;
		HeapNode *sibling = currentNode->sibling;
		int currentMinValue = currentMinimumNode->key;

		// traverse to find minimim
		while (currentNode != NULL && currentNode->sibling != NULL)
		{
			// check for lesser key value
			if (currentMinValue <= sibling->key)
			{
				currentNode = sibling;
				sibling = currentNode->sibling;
				//continue withou  chaging minimum
			}
			else
			{

				currentMinimumNode = sibling;
				currentMinimumNodePrev = currentNode;
				currentMinValue = sibling->key;
				currentNode = sibling;
				sibling = currentNode->sibling;

			} //continue by changing minimum
		}

		if (currentMinimumNodePrev == NULL)
		{
			T->heapHead = currentMinimumNode->sibling;
			T->heapHead = heapUnion(T->heapHead, revertHeap(currentMinimumNode->childNode));
			T->size--;
			return currentMinimumNode;
		}
		else
		{
			currentMinimumNodePrev->sibling = currentMinimumNode->sibling;

			T->heapHead = heapUnion(T->heapHead, revertHeap(currentMinimumNode->childNode));
			HeapNode *prev = NULL;
			HeapNode *current = T->heapHead;
			HeapNode *next = T->heapHead->sibling;
			// merge for nodes of same degree
			while ((current != NULL) && (next != NULL))
			{
				// printf("\nheadKey: %d", T->heapHead->key);
				// printf("\nsiblingKey: %d", T->heapHead->sibling->key);
				if (current->degree == next->degree)
				{
					current = minHeapMerge(current, next);
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
			T->size--;
			return currentMinimumNode;
		}
	}
	else
	{
		return NULL;
	}
}

//The complexity for Min() function is: O(logn)
//it is based on the height of the binomial heap
int Min(BinomialHeap *T)
{
	HeapNode *current = T->heapHead;
	int minimum = current->key;

	// compare for the minimum key
	while (current != NULL && current->sibling != NULL)
	{
		if (minimum < current->sibling->key)
		{
			//case 1
			minimum = minimum;
		}
		else
		{
			//case 2
			minimum = current->sibling->key;
		}
		current = current->sibling;
	}
	return minimum;
}

// Complexity fir TaskScheduler(): The operations in taskschedler takes O(logn)
// which is performed in sequence. therfore, time complexity which is done n
//  times as our inputs. thus the complexity is :O(nlogn)
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
		inputCount = fscanf(inputFile, "%d %d %d %d", &name, &excecutionT, &releaseT, &deadline);
		while (inputCount != EOF) //check if input completed
		{
			if (inputCount == 4) // check if there are set of four values for each task
			{					 //TODO: insert to binary heap
				//release time as key
				Insert(releaseTHeap, releaseT, name, excecutionT, releaseT, deadline);
				// printf("%d %d %d %d\n", name, excecutionT, releaseT, deadline);
				inputCount = fscanf(inputFile, "%d %d %d %d", &name, &excecutionT, &releaseT, &deadline);
			}
			else
			{
				printf("Error: input error when reading the attribute of the task %s", f1);
				exit(-1);
			}
		}
	}

	// binomial heaps for schedules points and deadline values as keys
	BinomialHeap *scheduleHeap = newHeap();
	int minReleaseTime = Min(releaseTHeap);

	int i; //Insert by number of cores
	for (i = m; i >= 1; i--)
	{
		Insert(scheduleHeap, minReleaseTime, i, 0, 0, 0);
	}
	fclose(inputFile);
	FILE *outputFile = fopen(f2, "w+"); //file to write output,delete pre-existing
	//and write new
	assert(outputFile != NULL);
	BinomialHeap *deadlineHeap = newHeap();
	while (releaseTHeap->size > 0)
	{
		//check task
		while (releaseTHeap->size > 0 && Min(releaseTHeap) <= minReleaseTime)
		{
			HeapNode *task = RemoveMin(releaseTHeap);
			if (task)
			{ //insert to  deadline Heap
				Insert(deadlineHeap, task->Dline, task->TaskName, task->Etime, task->Rtime, task->Dline);
			}
		}
		if (!deadlineHeap->size)
		{
			minReleaseTime = Min(releaseTHeap);
			while (Min(scheduleHeap) < minReleaseTime)
			{
				HeapNode *t = RemoveMin(scheduleHeap);
				Insert(scheduleHeap, minReleaseTime, t->TaskName, 0, 0, 0);
			}
		}
		while (deadlineHeap->size > 0)
		{

			// check for availavble for task
			while (releaseTHeap->size > 0 && Min(releaseTHeap) <= Min(scheduleHeap))
			{
				HeapNode *nextMinReleaseTime = RemoveMin(releaseTHeap);
				if (nextMinReleaseTime)
				{
					Insert(deadlineHeap, nextMinReleaseTime->Dline, nextMinReleaseTime->TaskName, nextMinReleaseTime->Etime, nextMinReleaseTime->Rtime, nextMinReleaseTime->Dline);
				}
			}
			HeapNode *nextMinDline = RemoveMin(deadlineHeap);
			HeapNode *nextCoreToPut = RemoveMin(scheduleHeap);
			if (nextMinDline && nextCoreToPut)
			{
				// Time at whih task ends
				int endTime = nextCoreToPut->key + nextMinDline->Etime;
				// If it exceed the possible time
				if (endTime > nextMinDline->Dline)
				{
					fclose(outputFile); //close the file if schedule not fixed
					return 0;
				}
				else
				{
					Insert(scheduleHeap, endTime, nextCoreToPut->TaskName, 0, 0, 0);
					// Write the possible output to file
					fprintf(outputFile, "%d core%d %d\n", nextMinDline->TaskName, nextCoreToPut->TaskName, nextCoreToPut->key);
				}
			}
		}
		minReleaseTime = Min(scheduleHeap);
	}
	fclose(outputFile); //close the file if schedule fixed
	return 1;			// feasible schedule true
}

int main() //sample main for testing
{
	int i;
	i = TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
	if (i == 0)
		printf("No feasible schedule!\n");
	/* There is a feasible schedule on 4 cores */
	i = TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
	if (i == 0)
		printf("No feasible schedule!\n");
	/* There is no feasible schedule on 3 cores */
	i = TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
	if (i == 0)
		printf("No feasible schedule!\n");
	/* There is a feasible schedule on 5 cores */
	i = TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
	if (i == 0)
		printf("No feasible schedule!\n");
	/* There is no feasible schedule on 4 cores */
	i = TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
	if (i == 0)
		printf("No feasible schedule!\n");
	/* There is no feasible schedule on 2 cores */
	i = TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
	if (i == 0)
		printf("No feasible schedule!\n");
	/* There is a feasible schedule on 2 cores */
	return 0;
}

// Helper functions defenitions

//Merge the heap in min-heap  rule with key
HeapNode *minHeapMerge(HeapNode *node1, HeapNode *node2)
{
	if (!node1)
		return node2;
	if (!node2)
		return node1;
	//check merges based on key
	if (node1->key < node2->key)
	{
		//case1
		node1->sibling = node2->sibling;
		node2->sibling = node1->childNode;
		node1->childNode = node2;
		node1->degree++;
		return node1;
	}
	else if ((node1->key == node2->key) && (node1->TaskName < node2->TaskName))
	{
		//case 2
		node1->sibling = node2->sibling;
		node2->sibling = node1->childNode;
		node1->childNode = node2;
		node1->degree++;
		return node1;
	}
	else
	{
		//case3
		node1->sibling = node2->childNode;
		node2->childNode = node1;
		node2->degree++;
		return node2;
	}
}
//relinks the nodes after deletion and keeps the heap valid
HeapNode *revertHeap(HeapNode *heapRoot)
{
	if (heapRoot == NULL)
	{
		return heapRoot;
	}
	HeapNode *prev = NULL;
	HeapNode *sibling = heapRoot->sibling;
	// traverse till the end
	while (heapRoot != NULL && sibling != NULL)
	{
		heapRoot->sibling = prev;
		prev = heapRoot;
		heapRoot = sibling;
		sibling = heapRoot->sibling;
	}
	heapRoot->sibling = prev;
	return heapRoot;
}

//combine two heaps
HeapNode *heapUnion(HeapNode *node1, HeapNode *node2)
{
	if (node1 == NULL && node2 == NULL)
	{
		return NULL;
	}
	if (!node1)
	{
		return node2;
	}
	if (!node2)
	{
		return node1;
	}
	else
	{
		HeapNode *prevNode;
		HeapNode *nextNode;
		HeapNode *auxilaryNode1 = node1;
		HeapNode *auxilaryNode2 = node2;
		if (node1->degree >= node2->degree)
		{
			prevNode = node2;
			auxilaryNode2 = node2->sibling;
		}
		else
		{
			prevNode = node1;
			auxilaryNode1 = node1->sibling;
		}
		nextNode = prevNode;
		//check merges based on degree
		while (auxilaryNode1 && auxilaryNode2)
		{
			// case 1
			if (auxilaryNode1->degree < auxilaryNode2->degree)
			{
				nextNode->sibling = auxilaryNode1;
				auxilaryNode1 = auxilaryNode1->sibling;
			}
			else if ((auxilaryNode1->degree == auxilaryNode2->degree) && (auxilaryNode1->TaskName < auxilaryNode2->TaskName))
			{
				nextNode->sibling = auxilaryNode1;
				auxilaryNode1 = auxilaryNode1->sibling;
			}
			else
			{
				// case 2
				nextNode->sibling = auxilaryNode2;
				auxilaryNode2 = auxilaryNode2->sibling;
			}
			nextNode = nextNode->sibling;
		}
		if (auxilaryNode1)
			nextNode->sibling = auxilaryNode1;

		if (auxilaryNode2)
			nextNode->sibling = auxilaryNode2;

		return prevNode;
	}
}
