#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<conio.h>

typedef int T;
typedef struct node {
	T data;
	node *left, *right;
	int height, balance;
} node;

int reHeightAll(node*);
void reBalance(node**);

void init(node **n, T data){
	(*n)->data = data;
	(*n)->left = NULL;
	(*n)->right = NULL;
	(*n)->balance = 0;
	(*n)->height = 1;
}

node *createNode(T data){
	node * n = (node *)malloc(sizeof(node));
	init(&n, data);
	return n;
}

void _reBalance_case(node **pr, int mode){
	node *x = *pr, *y, *z, *t1, *t2;
	switch (mode){
	case 0:
		y = x->left;
		z = y->left;
		t2 = y->right;

		y->left = z;
		y->right = x;
		x->left = t2;
		*pr = y;
		break;
	case 1:
		y = x->left;
		z = y->right;
		t1 = z->left;
		t2 = z->right;

		z->left = y;
		z->right = x;
		y->right = t1;
		x->left = t2;
		*pr = z;
		break;
	case 2:
		y = x->right;
		z = y->right;
		t1 = y->left;

		y->left = x;
		y->right = z;
		x->right = t1;
		*pr = y;
		break;
	case 3:
		y = x->right;
		z = y->left;
		t1 = z->left;
		t2 = z->right;

		z->left = x;
		z->right = y;
		x->right = t1;
		y->left = t2;
		*pr = z;
		break;
	}
}

void _reBalance(node **pr){
	if (*pr == NULL) return;
	if ((*pr)->balance > 1){
		if ((*pr)->left->balance >= 1)
			_reBalance_case(pr, 0);
		else
			_reBalance_case(pr, 1);

		reHeightAll(*pr);
	}
	else if ((*pr)->balance < -1){
		if ((*pr)->right->balance <= -1)
			_reBalance_case(pr, 2);
		else
			_reBalance_case(pr, 3);

		reHeightAll(*pr);
	}
	reBalance(&(*pr)->left);
	reBalance(&(*pr)->right);
}

int isBalance(node*);
void reBalance(node **pr){
	do{
		reHeightAll(*pr);
		_reBalance(pr);
		reHeightAll(*pr);
	} while (!isBalance(*pr));
}

int height(node *);
int reHeightAll(node *r){
	if (r == NULL) return 0;
	int left = reHeightAll(r->left), right = reHeightAll(r->right);
	r->height = (left > right ? left : right) + 1;
	r->balance = left - right;
	return r->height;
}

void addI(node **pr, T data){
	node **l = pr;
	int lr = 0;
	while (*l != NULL){
		lr = data < (*l)->data;
		l = lr ? &(*l)->left : &(*l)->right, data;
	}
	*l = createNode(data);
	reBalance(pr);
}

void _add(node **pr, T data){
	if (*pr == NULL)
		*pr = createNode(data);
	else {
		int lr = data < (*pr)->data;
		_add(lr ? &(*pr)->left : &(*pr)->right, data);
	}
}

void add(node **pr, T data){
	_add(pr, data);
	reBalance(pr);
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
	if (r->data == data) return height(r);
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

void printSpace(int space, int * sack){
	int i = 0;
	while (space-- > 8){
		if (space % 10 == 7 && sack[i++] == 0){
			printf("|");
		}
		else printf(" ");
	}
	while (space-- > 6) printf("|");
	while (space-- > 2) printf("-");
	while (space-- > 0) printf(">");
}

int toggle(int i){
	return (i + 1) % 2;
}

void _print(node *r, int level, int * sack){
	if (r != NULL){
		int old_sack = sack[level - 1];
		if (level > 0)
			sack[level - 1] = old_sack ? 0 : 1;
		sack[level] = 0;
		_print(r->right, level + 1, sack);

		printSpace(7 + 10 * level, sack);
		printf(" %3d %c\n", r->data, r->left == NULL && r->right == NULL ? ' ' : '>');

		if (level > 0)
			sack[level - 1] = old_sack;
		sack[level] = 1;
		_print(r->left, level + 1, sack);
	}
}

void print(node *r){
	int * sack = (int *)malloc(sizeof(int) * height(r));
	_print(r, 0, sack);
	free(sack);
}

T findMaxAndRemove(node **);

void _remove(node **pr, T data){
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
		_remove(data < (*pr)->data ? &(*pr)->left : &(*pr)->right, data);
}

int isBalance(node *pr){
	if (pr == NULL) return 1;
	if (pr->balance > 1 || pr->balance < -1) return 0;
	return !(!isBalance(pr->left) || !isBalance(pr->right));
}

void remove(node **pr, T data){
	_remove(pr, data);
	reBalance(pr);
}

T findMaxAndRemove(node **pr){
	if ((*pr)->right == NULL){
		T i = (*pr)->data;
		if ((*pr)->left != NULL)
			_remove(pr, (*pr)->data);
		else free(*pr);
		*pr = NULL;
		return i;
	}
	else
		return findMaxAndRemove(&(*pr)->right);
}

void add(node **pr, int count, T a, ...){
	va_list va;
	va_start(va, count);
	while (count-- > 0){
		T arg = va_arg(va, T);
		add(pr, arg);
	}
	va_end(va);
}

// Queue
typedef struct nodeQ {
	node * data;
	nodeQ * next;
} nodeQ;

nodeQ *createNodeQueue(node *data, nodeQ *next){
	nodeQ *ret = (nodeQ*)malloc(sizeof(nodeQ));
	ret->data = data;
	ret->next = next;
	return ret;
}

void enQueue(nodeQ **q, node *data){
	*q = createNodeQueue(data, *q);
}

node *deQueue(nodeQ **q){
	if ((*q) == NULL) return NULL;
	if ((*q)->next == NULL){
		node* p = (*q)->data;
		*q = NULL;
		return p;
	} else return deQueue(&(*q)->next);
}

void breadthFirstSearch(node *n){
	if (n == NULL) return;
	printf("- BreadthFirstSearch : ");

	node * use;
	nodeQ * queue = NULL;
	enQueue(&queue, n);
	while ((use = deQueue(&queue)) != NULL){
		printf("%d ", use->data);
		if (use->left != NULL)
			enQueue(&queue, use->left);
		if (use->right != NULL)
			enQueue(&queue, use->right);
	}
}

int main(){
	node * r = createNode(100);
	for (int i = 97; i <= 104; i++){
		if (i == 100) continue;
		add(&r, i);
	}
	//add(&r, 14, 50, 150, 25, 175, 12, 188, 30, 31, 32, 33, 34, 35, 37, 39);
	print(r);
	breadthFirstSearch(r);
	printf("\n");
	return 0;
}