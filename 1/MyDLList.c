// Author: Amel Johny
// Student ID: 5294308
// Platform: Windows 10 , GCC compiler 4.8.3
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data structures representing DLList

// data type for nodes
typedef struct DLListNode{
	int value; // value (int) of this list item
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int size;		   // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it){
	DLListNode *new;
	
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList(){
	DLList *L;

	L = malloc(sizeof(struct DLList));
	assert(L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// Helper function to add element into a list
// Time complexity analysis for addToList: The process has finite steps
// of O(1) time complexity.Therefore it has time complexity of order of O(1).
DLList *addToList(DLList *list, int value){
	if (list->first == NULL){
		list->first = newDLListNode(value);
		list->last = list->first;
	}
	else{
		list->last->next = newDLListNode(value);
		list->last->prev = list->last;
		list->last = list->last->next;
	}
	list->size++;
	return list;
}

// Helper function to check for same element in two lists
// Time complexity analysis for addToList: The process has m steps where m is
// the number of elements. Therefore it has time complexity of order of O(m).

int checkForSameElement(DLListNode *list1Node, DLListNode *list2Node, int flag){
	while (list1Node != NULL && !flag){
		if (list1Node->value == list2Node->value){
			flag = 1;
		}
		list1Node = list1Node->next;
	}
	return flag;
}

// create a DLList from a text file
// Time complexity analysis for CreateDLListFromFileDlist(): The time 
// complexity is of the order of O(n) where each if clause can have n 
// iterations depending on the number of inputs neglecting the statements 
// with O(1) compared to the higher magnitude of n.
// (3 + n*3) + (3 + n*2) -> 6 + 5n -> 5n -> n -> O(n)
DLList *CreateDLListFromFileDlist(const char *filename){
	// put your code here
	if (!strcmp(filename,"stdin")){
		DLList *list = newDLList();
		char buffer[64] = "";
		
		while (strcmp(buffer, "end") != 0){
			scanf("%s", buffer);
			// number = atoi(buffer);
			if (strcmp(buffer, "end")){
				list = addToList(list, atoi(buffer));
			}
		}
		return list;
	}else{
		DLList *list = newDLList();
		FILE *filePointer = fopen(filename, "r");
		if(filePointer == NULL){
			printf("\n%s does not exist\n",filename);
			return list;
		}
		int number;
		while (fscanf(filePointer, "%d", &number) != EOF){
			list = addToList(list, number);
		}
		return list;
	}
	//
}

// clone a DLList
// Time complexity analysis for cloneList(): The function has a while loop
// which occurs n time considering the number is elements of the list as n.
// Therefore the timecomplexity is in the order of O(n).
// 3 + n -> n -> O(n)
DLList *cloneList(DLList *u){
	// put your code here
	DLListNode *current = u->first;
	DLList *newList = newDLList();

	while (current != NULL){
		newList = addToList(newList, current->value);
		current = current->next;
	}
	return newList;
}

// compute the union of two DLLists u and v
// Time complexity analysis for setUnion() : Time Complexity is of the order
// O(n*m) considering n,m as the size of u and v respecively, where the
// for loop occurs n times to loop the initial list and
// each forloop has m occurance of while loop inside the checkForSameElement()
// function. The time complexity can be considered of order O(n^2)
// 6 + (n*(4 + m)) -> 6 + (4*n) + (n*m) -> (n*n) -> O(n^2)
DLList *setUnion(DLList *u, DLList *v){
	// put your code here
	DLList *newList = cloneList(u);
	DLListNode *currentPointerU = u->first;
	DLListNode *currentPointerV = v->first;
	int i = 0, flag = 0;

	for (i; i < v->size; i++){
		flag = checkForSameElement(currentPointerU, currentPointerV, flag);
		if (!flag){
			newList = addToList(newList, currentPointerV->value);
		}
		currentPointerV = currentPointerV->next;
		currentPointerU = u->first;
		flag = 0;
	}
	return newList;
}

// compute the insection of two DLLists u and v
// Time complexity analysis for intersection() : Time Complexity is of order
// O(n*m) depending on the number of items in the list
// considering the list size of u as n and v as m, the for loop occurs n
// times and each loop has m while loop occurance to check intersecting values
// in the chechForSameElement() function.For high magnitude of n,m the 
// time complexity can be considered as order O(n^2)
// 5 +(n*(5 + m)) -> 5 + 5*n + (n*m) -> (n*n) -> O(n^2)

DLList *setIntersection(DLList *u, DLList *v){
	// put your code here
	DLList *newList = newDLList();
	int i = 0, flag = 0;
	DLListNode *currentPointerU = u->first;
	DLListNode *currentPointerV = v->first;
	
	for(i; i < v->size; i++){
		flag = checkForSameElement(currentPointerU, currentPointerV, flag);
		if (flag){
			newList = addToList(newList, currentPointerV->value);
		}
		currentPointerV = currentPointerV->next;
		currentPointerU = u->first;
		flag = 0;
	}
	return newList;
}

// free up all space associated with list
// put your time complexity analysis for freeDLList(): Time Complexity is of
// order O(n) depending on the number of items in the list since it is
// traversed linearly. n + 3 -> n -> O(n) 
void freeDLList(DLList *L){
	// put your code here
	DLListNode *temp;
	
	while (L->first != NULL){
		temp = L->first;
		L->first = L->first->next;
		free(temp);
	}
	L->size = 0;
	free(L);
}

// display items of a DLList
// Time complexity analysis for printDDList(): The time complexity is
// of the order O(n) where the function has a while loop that excecutes
// n times considering the number of elements in the list as n.
// n+2 -> n -> O(n)
void printDLList(DLList *u){
	// put your code here
	if (u->first != NULL){
		DLListNode *node = u->first;
	
		while (node != NULL){
			printf("%d\n", node->value);
			node = node->next;
		}
	
		printf("\n");
	}
}

int main(){
	DLList *list1, *list2, *list3, *list4;
	
	list1 = CreateDLListFromFileDlist("File1.txt");
	printf("\nList elements of File1.txt");
	printDLList(list1);

	list2 = CreateDLListFromFileDlist("File2.txt");
	printf("\nList elements of File2.txt");
	printDLList(list2);

	list3 = setUnion(list1, list2);
	printf("\nList elements of Union of list1 and list2:\n");
	printDLList(list3);

	list4 = setIntersection(list1, list2);
	printf("\nList elements of Inersection of list1 and list2:\n");
	printDLList(list4);

	freeDLList(list1);
	freeDLList(list2);
	freeDLList(list3);
	freeDLList(list4);

	printf("\nplease type all the integers of list1:\n");
	list1 = CreateDLListFromFileDlist("stdin");

	printf("\nplease type all the integers of list2:\n");
	list2 = CreateDLListFromFileDlist("stdin");

	list3 = cloneList(list1);
	printf("\nList elements of clone of list1:\n");
	printDLList(list3);
	list4 = cloneList(list2);
	printf("\nList elements of clone of list2:\n");
	printDLList(list4);

	freeDLList(list1);
	freeDLList(list2);
	freeDLList(list3);
	freeDLList(list4);

	return 0;
}
