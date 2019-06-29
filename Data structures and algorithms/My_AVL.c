#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkName = 0;

typedef struct NODE
{
	int height;
	//int sumLeft;
	//int sumRight;
	int val;
	struct NODE *left;
	struct NODE *right;
}NODE;


NODE *newNode(int value)
{
	//printf("New node open\n");
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	newNode->val = value;
	//newNode->sumLeft = 0;
	//newNode->sumRight = 0;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->height = 1;  // new node is initially added at leaf
						  //printf("New node ready\n");
	return (newNode);
}

NODE *rightRotate(NODE *y)
{
	//printf("Right Rotate\n");
	NODE *x = y->left;

	y->left = x->right;
	x->right = y;

	//y->height = maximum(height(y->left), height(y->right)) + 1;
	if (y->left != NULL && y->right != NULL)
		if (y->left->height > y->right->height)
			y->height = 1 + y->left->height;
		else y->height = 1 + y->right->height;

	else if (y->left != NULL && y->right == NULL)
		y->height = 1 + y->left->height;
	else if (y->left == NULL && y->right != NULL)
		y->height = 1 + y->right->height;
	else y->height = 1;

	//x->height = maximum(height(x->left), height(x->right)) + 1;
	if (x->left != NULL && x->right != NULL)
		if (x->left->height > x->right->height)
			x->height = 1 + x->left->height;
		else x->height = 1 + x->right->height;

	else if (x->left != NULL && x->right == NULL)
		x->height = 1 + x->left->height;
	else if (x->left == NULL && x->right != NULL)
		x->height = 1 + x->right->height;
	else x->height = 1;

	//y->sumLeft = x->sumRight;
	//x->sumRight = y->sumLeft + y->sumRight + 1;

	return x;
}



NODE *leftRotate(NODE *x)
{
	NODE *y = x->right;

	x->right = y->left;
	y->left = x;

	//x->height = maximum(height(x->left), height(x->right)) + 1;
	if (x->left != NULL && x->right != NULL)
		if (x->left->height > x->right->height)
			x->height = 1 + x->left->height;
		else x->height = 1 + x->right->height;

	else if (x->left != NULL && x->right == NULL)
		x->height = 1 + x->left->height;
	else if (x->left == NULL && x->right != NULL)
		x->height = 1 + x->right->height;
	else x->height = 1;

	//y->height = maximum(height(y->left), height(y->right)) + 1;
	if (y->left != NULL && y->right != NULL)
		if (y->left->height > y->right->height)
			y->height = 1 + y->left->height;
		else y->height = 1 + y->right->height;

	else if (y->left != NULL && y->right == NULL)
		y->height = 1 + y->left->height;
	else if (y->left == NULL && y->right != NULL)
		y->height = 1 + y->right->height;
	else y->height = 1;

	//x->sumRight = y->sumLeft;
	//y->sumLeft = x->sumLeft + x->sumRight + 1;

	return y;
}

NODE* insert(NODE* node, int value)
{

	if (node == NULL)
	{
		//	printf("newNode\n");
		return(newNode(value));
	}
	//printf("\nname of user is %s", node->nameOfUser);

	if (value == node->val)
	{
		checkName = 1;
		return node;
	}
	//if (key < node->key)
	if (value < node->val)
	{
		//	printf("1\n");
		node->left = insert(node->left, value);
		/*if (checkName == 0)
			node->sumLeft++;*/
	}
	//else if (key > node->key)
	else if (value > node->val)
	{
		//printf("2\n");
		node->right = insert(node->right, value);
		/*if (checkName == 0)
			node->sumRight++;*/
	}

	else
		return node;


	/*node->height = 1 + maximum(height(node->left),
		height(node->right));*/
	if (node->left != NULL && node->right != NULL)
		if (node->left->height > node->right->height)
			node->height = 1 + node->left->height;
		else node->height = 1 + node->right->height;

	else if (node->left != NULL && node->right == NULL)
		node->height = 1 + node->left->height;
	else if (node->left == NULL && node->right != NULL)
		node->height = 1 + node->right->height;
	else node->height = 1;

	//int balance = getBalance(node);
	int balance;
	if (node != NULL)
	{
		if (node->left != NULL && node->right != NULL)
			balance = (node->left->height) - (node->right->height);
		else if (node->left != NULL && node->right == NULL)
			balance = node->left->height;
		else if (node->left == NULL && node->right != NULL)
			balance = 0 - node->right->height;
		else balance = 0;
	}
	else balance = 0;

	// Left Left Case
	if ((balance > 1) && (value < node->left->val))
	{
		//	printf("Left Left Case\n");
		return rightRotate(node);
	}

	// Right Right Case
	if ((balance < -1) && (value > node->right->val))
	{
		//	printf("Right Right Case\n");
		return leftRotate(node);
	}
	// Left Right Case
	if ((balance > 1) && (value > node->left->val))
	{
		//printf("Left Right Case\n");
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if ((balance < -1) && (value < node->right->val))
	{
		//	printf("Right Left Case\n");
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

NODE *search(NODE *node, int value)
{
	if (node == NULL)
		return NULL;
	if (node->val == value)
		return node;
	if (node->val > value) search(node->left, value);
	else search(node->right, value);
}

void preOrder(NODE *root)
{
	if (root != NULL)
	{
		printf("%d ", root->val);
		preOrder(root->left);
		preOrder(root->right);
	}
}

/* Drier program to test above function*/
int main()
{
	NODE *root = NULL;

	int rand_arr[10001];
	int i;
	int rand_val;
	srand(time(NULL));
	for (i = 0; i < 10000; i++)
	{

		rand_val = 0 + rand() % 1000000;
		rand_arr[i] = rand_val;
		printf("%d\n", rand_arr[i]);
	}
	for (i = 0; i < 10000; i++)
	{
		insert(root, rand_arr[i]);
	}
	preOrder(root);

	/* Constructing tree given in the above figure */
	/*root = insert(root, 10);
	root = insert(root, 20);
	root = insert(root, 30);
	root = insert(root, 40);
	root = insert(root, 50);
	root = insert(root, 25);*/

	/* The constructed AVL Tree would be
	30
	/  \
	20   40
	/  \     \
	10  25    50
	*/

	printf("Preorder traversal of the constructed AVL"
		" tree is \n");
	preOrder(root);
	system("pause");

	return 0;
}