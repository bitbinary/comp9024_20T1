// Author: ************
// Student ID: ************
// Platform: Windows 10 , GCC compiler 4.8.3

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data type for avl tree nodes
typedef struct AVLTreeNode
{
	int key;					//key of this item
	int value;					//value (int) of this item
	int height;					//height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left;	//pointer to left child
	struct AVLTreeNode *right;	//pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree
{
	int size;		   // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v)
{
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0;  // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
	AVLTree *T;
	T = malloc(sizeof(AVLTree));
	assert(T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}

// HELPER FUNCTIONS DECLARATIONS
int height(AVLTreeNode *root);
AVLTreeNode *positionElementInTree(AVLTreeNode *root, int k, int v);
AVLTreeNode *rotateLeft(AVLTreeNode *root);
AVLTreeNode *rotateRight(AVLTreeNode *root);
void linkToParent(AVLTreeNode *parent, AVLTreeNode *child);
void unlinkFromParent(AVLTreeNode *parent, AVLTreeNode *child);
int checkIfBalanced(AVLTreeNode *root);
void balanceTree(AVLTree *T, AVLTreeNode *currentNode);
AVLTreeNode *cloneNode(AVLTreeNode *root);
void freeNode(AVLTreeNode *node);
AVLTreeNode *SearchTree(AVLTreeNode *root, int k, int v);
void checkForIntersection(AVLTree *new, AVLTree *T1, AVLTreeNode *t2);
void checkForDuplicate(AVLTree *newTree, AVLTree *T1, AVLTreeNode *t2);
AVLTreeNode *combineTrees(AVLTreeNode *node1, AVLTreeNode *node2);

// MAIN FUNCTIONS DECLARATIONS
AVLTreeNode *Search(AVLTree *T, int k, int v);
AVLTree *CreateAVLTree(const char *filename);
AVLTree *CloneAVLTree(AVLTree *T);
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2);
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2);
void FreeAVLTree(AVLTree *T);
void PrintNode(AVLTreeNode *root);
int DeleteNode(AVLTree *T, int k, int v);
void PrintAVLTree(AVLTree *T);
int InsertNode(AVLTree *T, int k, int v);

// Find the height of the tree node and returns the value
int height(AVLTreeNode *root)
{
	if (root == NULL)
	{
		return -1;
	}
	int h;
	if ((height(root->left)) >= (height(root->right)))
	{
		h = height(root->left);
	}
	else
	{
		h = height(root->right);
	}
	return h + 1;
}

// Position element correctly in the tree comparing values
AVLTreeNode *positionElementInTree(AVLTreeNode *root, int k, int v)
{
	if (root->key == k && root->value == v)
	{
		return NULL;
	}
	if ((root->key > k) || ((root->key == k) && (root->value > v)))
	{
		if (root->left == NULL)
		{
			AVLTreeNode *newNode = newAVLTreeNode(k, v);
			root->left = newNode; //insert to left
			newNode->parent = root;
			return newNode;
		}
		return positionElementInTree(root->left, k, v); // insert towards left
	}
	if ((root->key < k) || ((root->key == k) && (root->value < v)))
	{
		if (root->right == NULL)
		{
			AVLTreeNode *newNode = newAVLTreeNode(k, v);
			root->right = newNode; //insert to right
			newNode->parent = root;
			return newNode;
		}
		return positionElementInTree(root->right, k, v); //insert toeards right
	}
	else
	{
		return NULL;
	}
}
// Tree rotations left
AVLTreeNode *rotateLeft(AVLTreeNode *root)
{
	if (root == NULL || root->right == NULL)
	{
		return NULL;
	}
	AVLTreeNode *new = root->right;
	root->right = new->left;
	new->left = root;
	new->parent = root->parent;
	root->parent = new;
	if (root->right)
		root->right->parent = root;
	return new;
}
// Tree Rotate right
AVLTreeNode *rotateRight(AVLTreeNode *root)
{
	if (root == NULL || root->left == NULL)
	{
		return NULL;
	}
	AVLTreeNode *new = root->left;
	root->left = new->right;
	new->right = root;
	new->parent = root->parent;
	root->parent = new;
	if (root->left)
		root->left->parent = root;
	return new;
}
// link node to parent used for rotation and deletion
void linkToParent(AVLTreeNode *parent, AVLTreeNode *child)
{
	if ((child->key < parent->key) || ((child->key == parent->key) && (child->value < parent->value)))
	{
		parent->left = child;
		child->parent = parent;
	}
	if ((child->key > parent->key) || ((child->key == parent->key) && (child->value > parent->value)))
	{
		parent->right = child;
		child->parent = parent;
	}
}
// Unlink from parent used for isolating the node
void unlinkFromParent(AVLTreeNode *parent, AVLTreeNode *child)
{
	if (((child->key == parent->key) && (child->value < parent->value)) || (child->key < parent->key))
	{
		parent->left = NULL;
	}
	if (((child->key == parent->key) && (child->value > parent->value)) || (child->key > parent->key))
	{
		parent->right = NULL;
	}
}

// recursive function to check if tree is balanced
int checkIfBalanced(AVLTreeNode *root)
{
	int balanced = 0;
	if (root == NULL)
	{
		balanced = 1;
		return balanced;
	}
	if (checkIfBalanced(root->left) && checkIfBalanced(root->right))
	{
		balanced = abs(height(root->left) - height(root->right)) <= 1;
		return balanced;
	}
	return 0;
}

// Recursively clone the elemets in the left and right
AVLTreeNode *cloneNode(AVLTreeNode *root)
{
	if (root == NULL)
	{
		return root;
	}
	AVLTreeNode *newRootNode = newAVLTreeNode(root->key, root->value);
	newRootNode->left = cloneNode(root->left);
	newRootNode->right = cloneNode(root->right);
	return newRootNode;
}

// Helper function for for Freeing node
void freeNode(AVLTreeNode *node)
{
	if (node != NULL)
	{
		freeNode(node->left);
		freeNode(node->right);
		node->key = 0;
		node->value = 0;
		node->parent = NULL;
		free(node);
	}
}

// Function to search for an element inn the tree
AVLTreeNode *SearchTree(AVLTreeNode *root, int k, int v)
{
	if (root != NULL)
	{
		if (root->key == k && root->value == v)
		{
			return root;
		}
		if ((root->key > k) || (root->key == k && root->value > v))
		{
			return SearchTree(root->left, k, v);
		}
		if ((root->key < k) || (root->key == k && root->value < v))
		{
			return SearchTree(root->right, k, v);
		}
	}
	return root;
}

// Checks whether there exist any same node. Insert if no intersection
//time Complexity: the time for search => O(log n)
void checkForIntersection(AVLTree *new, AVLTree *T1, AVLTreeNode *t2)
{
	if (t2 == NULL)
	{
		return;
	}
	AVLTreeNode *found = Search(T1, t2->key, t2->value);
	if (found)
	{
		InsertNode(new, t2->key, t2->value);
	}
	checkForIntersection(new, T1, t2->left);
	checkForIntersection(new, T1, t2->right);
}
// function to check if the element is a duplicate of current node.
//time Complexity: the time for search => O(log n)

void checkForDuplicate(AVLTree *newTree, AVLTree *T1, AVLTreeNode *t2)
{
	if (t2 == NULL)
	{
		return;
	}
	AVLTreeNode *found = Search(T1, t2->key, t2->value);
	if (!found)
	{
		InsertNode(newTree, t2->key, t2->value);
	}
	checkForDuplicate(newTree, T1, t2->left);
	checkForDuplicate(newTree, T1, t2->right);
}

// Functions combines two subtrees to help deletion.
AVLTreeNode *combineTrees(AVLTreeNode *node1, AVLTreeNode *node2)
{
	AVLTreeNode *parent = NULL, *current;
	current = node2;
	if (!node1)
	{
		return node2;
	}
	else if (!node2)
	{
		return node1;
	}
	while (current->left)
	{
		parent = current;
		current = current->left;
	}
	if (!parent)
	{
		parent->left = current->right;
		if (parent->left)
		{
			parent->left->parent = parent;
		}
		current->right = node2;
	}
	current->left = node1;
	return current;
}

// Balance the AVL tree using LL,LR,RL,RR
// Time complexity : The function depends the height of the tree to
// balance the tree which is then of the order O(log n).
void balanceTree(AVLTree *tree, AVLTreeNode *currentNode)
{
	while (currentNode)
	{
		if (!checkIfBalanced(currentNode))
		{
			if (height(currentNode->left) > height(currentNode->right))
			{
				if (height(currentNode->left->left) > height(currentNode->left->right))
				{
					currentNode = rotateRight(currentNode);
				}
				else
				{
					currentNode->left = rotateLeft(currentNode->left);
					currentNode = rotateRight(currentNode);
				}
			}
			else
			{
				if (height(currentNode->right->left) > height(currentNode->right->right))
				{
					currentNode->right = rotateRight(currentNode->right);

					currentNode = rotateLeft(currentNode);
				}
				else
				{
					currentNode = rotateLeft(currentNode);
				}
			}
			if (currentNode->parent == NULL)
			{
				tree->root = currentNode;
			}
			else
			{
				linkToParent(currentNode->parent, currentNode);
			}
		}
		currentNode = currentNode->parent;
	}
}
// Time complexity analysis of CreateAVLTree()
// The process initiall collects the data to be added using a while loop
// and then inserts the data in each loop. The processes has the complexity of
// order O(n) and O(log n) therfore
// => O(n * log n)
AVLTree *CreateAVLTree(const char *filename)
{
	// put your code here
	AVLTree *newTree = newAVLTree();
	if (!strcmp(filename, "stdin"))
	{
		FILE *temp = fopen("temporary.txt", "w+");
		char buffer[64] = "";
		while (strcmp(buffer, "end") != 0)
		{
			scanf("%s", buffer);
			// number = atoi(buffer);
			if (strcmp(buffer, "end"))
			{
				// printf("%s\n", buffer);
				fputs(buffer, temp);
			}
		}
		fclose(temp);
		FILE *filePointer = fopen("temporary.txt", "r");
		if (filePointer == NULL)
		{
			printf("\n%s does not exist\n", "temporary.txt");
			return newTree;
		}
		int key, value;
		while (fscanf(temp, "(%d,%d)", &key, &value) == 2) // check for number of conversions
		{
			InsertNode(newTree, key, value);
		}
		fclose(filePointer);
		return newTree;
	}
	else
	{
		FILE *filePointer = fopen(filename, "r");
		if (filePointer == NULL)
		{
			printf("\n%s does not exist\n", filename);
			return newTree;
		}
		int key, value;
		while (fscanf(filePointer, " (%d,%d)", &key, &value) == 2) // check for number of conversions
		{
			InsertNode(newTree, key, value);
		}
		fclose(filePointer);
		return newTree;
	}
}

// Time complexity analysis for CloneAVLTree():
// The functions traverses through each element once
// therefore => O(n)
AVLTree *CloneAVLTree(AVLTree *T)
{
	AVLTree *newTree = newAVLTree();
	newTree->root = cloneNode(T->root);
	newTree->size = T->size;
	return newTree;
}

// Time complexity for ALVTreesUNion()
// The function initially takes one tree and clones it and then searches the
// other tree for non identical elements; Therefore Complexity is
// => m(clone) + mlogn(search) => O(m+mlog n)
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
	if (T1 == NULL || T1->root == NULL)
	{
		return T2;
	}
	else if (T2 == NULL || T2->root == NULL)
	{
		return T1;
	}
	AVLTree *newTree = CloneAVLTree(T1); // m

	checkForDuplicate(newTree, T1, T2->root); // mlogn
	return newTree;
}

// Time complexity for ALVTreesIntersection()
// The process for intersection searches each element of one tree
// with the elements of other tree. Therefore the complexity depends on
// the size of the trees(m,n). m * search Complexity of n
// => m*logn => O(mlog n)
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
	if (T1 == NULL || T1->root == NULL || T2 == NULL || T2->root == NULL)
	{
		return NULL;
	}

	AVLTree *newTree = newAVLTree();
	checkForIntersection(newTree, T1, T2->root); //mlogn
	return newTree;
}

// Time complexity analysis for InsertNode()
// The process of insertion initially finds the position to
// insert the element and then balance the tree.The proccesses has time
// complexities of O(log n) and O(log n),therfore
// => O(log n) + O (log n) => O(log n)
int InsertNode(AVLTree *T, int k, int v)
{
	//put your code here
	if (T->root == NULL)
	{
		T->root = newAVLTreeNode(k, v);
		return 1;
	}
	AVLTreeNode *newNode = positionElementInTree(T->root, k, v); //O(log n)
	//checck if insertion succesfull
	if (newNode == NULL)
	{
		return 0; //fail
	}
	else
	{
		T->size++;
		AVLTreeNode *currentNode = newNode->parent;
		balanceTree(T, currentNode); //O(log n)
		return 1;
	}
}

// Time complexity analysis for Search()
// The function traverse the tree by comparing the values of the nodes and
// thus not visit every  element. It visits are based on the height of the tree
// which is of the order of logn => complexity of order O(log n)
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
	if (T == NULL || T->root == NULL)
	{
		return NULL;
	}
	else
	{
		return SearchTree(T->root, k, v); //O(log n)
	}
}

// Time complexity analysis for freeAVLTree()
// The function visits everynode once. therefore the complexity is
// of the order O(n)
void FreeAVLTree(AVLTree *T)
{
	freeNode(T->root);
	T->size = 0;
	free(T);
}

// functions helps to print the elements by recurring itself in both directions
// The time of excecution of itself is of order O(n)
void PrintNode(AVLTreeNode *root)
{
	if (root != NULL)
	{
		PrintNode(root->left);
		printf("P: (%d , %d ) H: %d\n", root->key, root->value, height(root));

		PrintNode(root->right);
	}
}

// Time complexity for DeleteNode()
// The Delete operation initially searches for the element and then
// rmoves and balances the tree. The processes have time complexity
// of order O(log n) and o(log n).
// => O(log n) + O(log n) => O(log n)
int DeleteNode(AVLTree *T, int k, int v)
{
	AVLTreeNode *node = Search(T, k, v);

	if (node == NULL) // no element found
	{
		return 0;
	}
	AVLTreeNode *parent = node->parent;
	if (node->left == NULL && node->right == NULL)
	{
		if (parent)
		{
			unlinkFromParent(node->parent, node);
		}
		free(node);

		balanceTree(T, parent);
		return 1;
	}
	if (!(node->left))
	{
		if (parent)
		{
			linkToParent(node->parent, node->right);
		}
		else
		{
			node->right->parent = NULL;
			T->root = node->right;
		}
		free(node);
		balanceTree(T, parent);
		return 1;
	}
	if (!(node->right))
	{
		if (parent)
		{
			linkToParent(node->parent, node->left);
		}
		else
		{
			node->left->parent = NULL;
			T->root = node->left;
		}
		free(node);
		balanceTree(T, parent);
		return 1;
	}

	if (parent != NULL)
	{
		linkToParent(node->parent, combineTrees(node->left, node->right));
	}
	else
	{
		T->root = combineTrees(node->left, node->right);
	}
	free(node);
	balanceTree(T, parent);
	return 1;
}

// Time complexity analysis for PrintAVLTree():
// The function traverse through the tree one time.
// Therefore the complexity is :  O(n)

void PrintAVLTree(AVLTree *T)
{
	// put your code here
	assert(T != NULL);
	printf("\n>>>>>>>>>>>>>>>>>>>\n");
	PrintNode(T->root);
	printf("<<<<<<<<<<<<<<<<<<<\n");
}

int main() //sample main for testing
{
	int i, j;
	AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
	AVLTreeNode *node1;

	tree1 = CreateAVLTree("stdin");
	PrintAVLTree(tree1);
	printf("Freeing tree 1");
	FreeAVLTree(tree1);
	// you need to create the text file file1.txt
	// to store a set of items without duplicate items
	tree2 = CreateAVLTree("file1.txt");
	printf("\nPrinting tree made from file1.txt");
	PrintAVLTree(tree2);
	tree3 = CloneAVLTree(tree2);
	printf("\nPrinting Clone");
	PrintAVLTree(tree3);
	FreeAVLTree(tree2);
	FreeAVLTree(tree3);
	//Create tree4
	tree4 = newAVLTree();
	j = InsertNode(tree4, 10, 10);
	for (i = 0; i < 15; i++)
	{
		j = InsertNode(tree4, i, i);
		if (j == 0)
			printf("\n(%d, %d) already exists\n", i, i);
	}
	PrintAVLTree(tree4);
	node1 = Search(tree4, 20, 20);
	if (node1 != NULL)
		printf("key= %d value= %d\n", node1->key, node1->value);
	else
		printf("Key 20 does not exist\n");

	for (i = 17; i > 0; i--)
	{
		j = DeleteNode(tree4, i, i);
		if (j == 0)
			printf("\n --> Key %d does not exist\n", i);
		PrintAVLTree(tree4);
	}
	FreeAVLTree(tree4);
	//Create tree5
	tree5 = newAVLTree();
	j = InsertNode(tree5, 6, 25);
	j = InsertNode(tree5, 6, 10);
	j = InsertNode(tree5, 6, 12);
	j = InsertNode(tree5, 6, 20);
	j = InsertNode(tree5, 9, 25);
	j = InsertNode(tree5, 10, 25);
	PrintAVLTree(tree5);
	//Create tree6
	tree6 = newAVLTree();
	j = InsertNode(tree6, 6, 25);
	j = InsertNode(tree6, 5, 10);
	j = InsertNode(tree6, 6, 12);
	j = InsertNode(tree6, 6, 20);
	j = InsertNode(tree6, 8, 35);
	j = InsertNode(tree6, 10, 25);
	PrintAVLTree(tree6);
	tree7 = AVLTreesIntersection(tree5, tree6);
	tree8 = AVLTreesUnion(tree5, tree6);
	printf("Printing after intersection");

	PrintAVLTree(tree7);
	printf("Printing after Union");

	PrintAVLTree(tree8);
	return 0;
}
