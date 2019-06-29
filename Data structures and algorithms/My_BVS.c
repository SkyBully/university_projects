#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//int deep = 0;

typedef struct node {
	int val;
	//int deep;
	//struct node *parent;
	struct node *left;
	struct node *right;
};

struct node *newNode(int value)
{
	struct node* Node = (struct node*)malloc(sizeof(struct node));
	//Node->deep = 0;
	Node->val = value;
	Node->left = NULL;
	Node->right = NULL;
	return Node;

}

struct node *insert(struct node *Node, int value)
{
	//deep++;
	printf("BVS\n");
	if (Node == NULL)
		return newNode(value);
	if (value < Node->val)
		Node->left = insert(Node->left, value);
	else if (value > Node->val)
		Node->right = insert(Node->right, value);
	return Node;
}

struct node *search(struct node *Node, int value)
{
	if (Node == NULL) return NULL;
	if (Node->val == value) return Node;

	if (value < Node->val)
		Node = search(Node->left, value);
	else if (value > Node->val)
		Node = search(Node->right, value);

}

struct node *delete(struct node *Node, int value)
{
	if (Node == NULL) return NULL;
	
	
	if (value < Node->val)
		Node->left = delete(Node->left, value);
	else if (value > Node->val)
		Node->right = delete(Node->right, value);
	
	/*while (Node->val != value)
		if (value < Node->val)
			Node = Node->left;
		else if (value > Node->val)
			Node = Node->right;*/
		
	if (Node->val == value)
	{
		if (Node->left == NULL && Node->right == NULL)
		{
			struct node *tmp = NULL;
			free(Node);
			return tmp;
			//Node = NULL;
		}
		else if (Node->left != NULL && Node->right == NULL)
		{
			struct node *tmp = Node->left;
			free(Node);
			return tmp;
			//Node = Node->left;
			//Node->val = Node->left->val;
			//Node->left = Node->left->left;
		}
		else if (Node->right != NULL && Node->left == NULL)
		{
			struct node *tmp = Node->right;
			free(Node);
			return tmp;
			//Node = Node->right;
			//Node->val = Node->right->val;
			//Node->right = Node->right->right;
		}
		else
		{
			struct node *tmp = Node->right;
			/*if (tmp->left == NULL)
			{

				free(Node);
				return tmp;
			}*/
			//else {
				while (tmp->left != NULL)
					tmp = tmp->left;
				Node->val = tmp->val;
				Node->right = delete(Node->right, tmp->val);
				//tmp = NULL;
			//}
		}

	}

	return Node;

}

void preorder(struct node *Node)
{
	if (Node != NULL)
	{
		printf("%d\n", Node->val);
		preorder(Node->left);
		preorder(Node->right);
	}
}

int main()
{
	// sem napis svoje riesenie
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
	struct node *rootBVS = NULL;
	/*struct node *rootAVL = NULL;
	struct node *rootRBT = NULL;*/

	rootBVS = insert(rootBVS, rand_arr[0]);
	for (i = 0; i < 10000; i++)
	{
		insert(rootBVS, rand_arr[i]);
	}
	preorder(rootBVS);
	//////////////////////////////////////////////////////////
	/*
	struct node *root = NULL;
	int tmp;
	scanf("%d", &tmp);
	//printf("%d\n", deep);
	root = insert(root, tmp);

	//insert(root, 1);
	insert(root, 70);
	insert(root, 60);
	insert(root, 80);
	//insert(root, 75);
	preorder(root);
	printf("-------------------\n");
	delete(root, 70);
	preorder(root);*/
	

	/*while (scanf("%d", &tmp) > 0)
	{
		//deep = 0;
		insert(root, tmp);
	}*/

	system("pause");

	return 0;
}