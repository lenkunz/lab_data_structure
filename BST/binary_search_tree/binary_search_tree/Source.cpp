#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

typedef int T;
typedef struct node {
	T data;
	node *left, *right;
} node;

void init(node **n, T data){
	(*n)->data = data;
	(*n)->left = NULL;
	(*n)->right = NULL;
}

node *createNode(T data){
	node * n = (node *)malloc(sizeof(node));
	init(&n, data);
	return n;
}

void addI(node **pr, T data){
	node **l = pr;
	while (*l != NULL)
		l = data < (*l)->data ? &(*l)->left : &(*l)->right, data;
	*l = createNode(data);
}

void add(node **pr, T data){
	if (*pr == NULL)
		*pr = createNode(data);
	else
		add(data < (*pr)->data ? &(*pr)->left : &(*pr)->right, data);
}

void inOrder(node *r){
	if (r == NULL) return;
	inOrder(r->left);
	printf("%d ", r->data);
	inOrder(r->right);
}

void preOrder(node *r){
	if (r == NULL) return;
	printf("%d ", r->data);
	preOrder(r->left);
	preOrder(r->right);
}

void postOrder(node *r){
	if (r == NULL) return;
	postOrder(r->left);
	postOrder(r->right);
	printf("%d", r->data);
}

node * search(node *r, T data){
	if (r == NULL) return NULL;
	if (r->data = data) return r;
	return search(data < r->data ? r->left : r->right, data);
}

int height(node *r){
	if (r == NULL) return 0;
	int left = height(r->left), right = height(r->right);
	return (left > right ? left : right) + 1;
}

int height(node *r, T data){
	if (r == NULL) return 0;
	if (r->data == data) return 1;
	int i = height(data < r->data ? r->left : r->right, data);
	return i ? ++i : 0;
}

void _path(node *r, T data){
	printf("%d ", r->data);
	if (r->data != data)
		_path(data < r->data ? r->left : r->right, data);
}

void path(node *r, T data){
	if (search(r, data) != NULL)
		_path(r, data);
	else printf("No data in this BST!");
}

void printSpace(int space){
	while (space-- > 0) printf(" ");
}

void print(node *r, int level){
	if (r != NULL){
		print(r->right, level + 1);
		printSpace(3 * level);
		printf("%d\n", r->data);
		print(r->left, level + 1);
	}
}

T findMaxAndRemove(node **);

void remove(node **pr, T data){
	if ((*pr)->data == data)
		if ((*pr)->left == NULL){
			node * right = (*pr)->right;
			free(*pr);
			*pr = right;
		}
		else if ((*pr)->left->right == NULL){
			node * left = (*pr)->left;
			(*pr)->data = (*pr)->left->data;
			(*pr)->left = left->left;
			free(left);
		}
		else
			(*pr)->data = findMaxAndRemove(&(*pr)->left);
	else
		remove(data < (*pr)->data ? &(*pr)->left : &(*pr)->right, data);
}

T findMaxAndRemove(node **pr){
	if ((*pr)->right == NULL){
		T i = (*pr)->data;
		if ((*pr)->left != NULL)
			remove(pr, (*pr)->data);
		else free(*pr);
		*pr = NULL;
		return i;
	}
	else
		return findMaxAndRemove(&(*pr)->right);
}

int main(){
	return 0;
}