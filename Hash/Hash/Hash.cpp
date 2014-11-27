#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

#ifndef INDEXFALSE
#define INDEXFALSE -1
#endif

#if !defined(TRUE) && !defined(FALSE)
#define FALSE 0
#define TRUE 1
#endif

#ifndef INIT_HASHTABLE_SIZE_RELATE
#define INIT_HASHTABLE_SIZE_RELATE 0.2
#endif

// ================================
//  About read the dictionary file
// ================================
typedef struct readFileReturn {
	char ** arrs;
	int size;
} readFileReturn;

/* Read the word from file buffer @f
 * | has
 */
char * readWord(FILE * f){
	int length = 5, maxLength = length, nowLength = length, r;
	char * c = (char *)malloc(sizeof(char)* length), *startC = c;

	if (c == NULL) return NULL;

	while (true) {
		r = fgetc(f);

		if (length-- == 1){
			length = maxLength;
			char * cev = (char *)realloc(startC, sizeof(char)* (nowLength += length));
			if (cev == NULL){
				free(startC);
				return NULL;
			}
			c = cev + (c - startC);
			startC = cev;
		}

		if ((*c = r) == ' ' || *c == '\n' || r == EOF)
			break;
		c++;
	}

	if (*c == EOF && c == startC){
		free(c);
		return NULL;
	}

	*c = '\0';
	startC = (char *)realloc(startC, nowLength - length);
	return startC;
}

/* Create word array from @filepath
 * | has
 *		#readFileReturn | has child
 *			this->arrs: Array of word
 *			this->size: Array size
 *		@filepath: path to the file | has property
 *			File character encoding allowed: ASCII, UTF-8
 *			Dictionary character: [A-Z] [a-z]
 *			Seperator character: [SPACE:' '] or [NEWLINE:'\n']
 *			File end: [SYSTEM_EOF:EOF]
 */
readFileReturn * readFile(char * filepath){
	// Create return
	readFileReturn * result = (readFileReturn *)malloc(sizeof(readFileReturn));

	int length = 100, maxLength = length, nowLength = length;
	char ** c = (char **)calloc(length, sizeof(char*)), ** startC = c;

	FILE * f = fopen(filepath, "rb");
	if (f == NULL){
		fclose(f);
		free(result);
		return NULL;
	}

	while ((*c = readWord(f)) != NULL){
		//printf("[DEBUG] Final word is %s\n", *c);
		if (length-- <= 1){
			length = maxLength;
			char ** cev = (char **)realloc(startC, sizeof(char *)* (nowLength += length));
			if (cev == NULL){
				free(startC);
				return NULL;
			}
			c = cev + (c - startC);
			startC = cev;
		}
		c++;
	}
	c--;

	//startC = (char **) realloc(startC, nowLength - length);
	result->arrs = startC;
	result->size = nowLength - length;
	return result;
}

// ==================
//  About hash table
// ==================
typedef struct HashTableNode {
	char * word;
	struct HashTableNode * next;
} HashTableNode;

typedef struct HashTable {
	HashTableNode ** arrs;
	int size;
	int collision, used;
} HashTable;

/* Create hash node 
 * | has
 *		#HashTableNode: a created object
 *		@word: word to be the value of an object
 *		@next: next HashTableNode | NULL
 */
HashTableNode * createHashNode(char * word, HashTableNode * next){
	HashTableNode* ret = (HashTableNode*)malloc(sizeof(HashTableNode));
	ret->word = word;
	ret->next = next;
	return ret;
}

HashTableNode * createHashNode(char * word){
	return createHashNode(word, NULL);
}

/* Does @number a prime 
 * | has
 *		#int: defined TRUE or FALSE
 *		@number: a number to check if it is prime
 */
int isPrime(int number){
	int find = number;
	find = sqrt((double)find);

	while (find > 1)
	if (number % find-- == 0)
		return FALSE;

	return TRUE;
}

/* Find integer greater than @number which is prime 
 * | has
 *		#int: Integer that is prime and greater than @number
 *		@number: Integer to find its greater prime
 */
int findNextPrime(int number){
	int c = number;
	while (isPrime(c) == FALSE){ c++; }
	return c;
}

// Find the space between ASCII 'A' and ASCII 'a'
int CHR_DIFF = 'a' - 'A';

/* Hash function, get the hashed integer
 * | has
 *		#int: hashed integer
 *		@word: string to hash
 *		@tableSize: hash table size
 */
int hash(char * word, int tableSize){
	char c;
	int hashed = 0;

	while (*word != '\0'){
		c = *word;
		if (c >= 'A' && c <= 'Z'){
			c += CHR_DIFF;
		}

		hashed = 37 * hashed + c;
		word++;
	}

	hashed %= tableSize;
	if (hashed < 0)
		hashed += tableSize;

	return hashed;
}

int countWordLength(char * word){
	int count = 0;
	while (*word++ != '\n') count++;
	return count;
}

char * recreateWordWithoutIndex(char * word, int index){
	char * c = (char *)malloc(sizeof(char) * 100);
	int ind = 0;
	while (*word != '\0'){
		if (ind != index){
			c[ind++] = *word++;
		}
		else{
			c[ind++] = '_';
			word++;
		}
	}

	c[ind] = '\0';

	return c;
}

#define ADDHASH_ERROR 0
#define ADDHASH_NEW 1
#define ADDHASH_COLLISION 2
int addHash(HashTable * hashTable, char * word){
	int hashInt = hash(word, hashTable->size);
	if (hashTable->arrs[hashInt] == NULL){
		hashTable->arrs[hashInt] = createHashNode(word);
		return ADDHASH_NEW;
	}
	else{
		hashTable->arrs[hashInt] = createHashNode(word, hashTable->arrs[hashInt]);
		if (hashTable->arrs[hashInt] == NULL)
			return ADDHASH_ERROR;
		return ADDHASH_COLLISION;
	}
}

// Create HashTable
HashTable * createHashTable(int size){
	size = findNextPrime(size);
	HashTable * hashTable = (HashTable *)malloc(sizeof(HashTable));
	hashTable->collision = 0;
	hashTable->used = 0;
	hashTable->size = size;
	hashTable->arrs = (HashTableNode **)calloc(size, sizeof(HashTableNode *));
	
	return hashTable;
}

HashTable * rehash(HashTable * hashTable){
	HashTable * hashTab = createHashTable(hashTable->size * 2);
	if (hashTab == NULL){
		free(hashTable);
		return NULL;
	}

	HashTableNode * hashTableNode = NULL;
	for (int i = 0; i < hashTable->size; i++){
		if (hashTable->arrs[i] != NULL){
			hashTableNode = hashTable->arrs[i];
			while (hashTableNode != NULL){
				switch (addHash(hashTab, hashTableNode->word)){
				case ADDHASH_ERROR:
					free(hashTab);
					free(hashTable);
					return NULL;
				case ADDHASH_COLLISION:
					hashTab->collision++;
					break;
				case ADDHASH_NEW:
					hashTab->used++;
					break;
				};
				hashTableNode = hashTableNode->next;
			}
		}
	}

	free(hashTable);
	return hashTab;
}

HashTable * HashTableProcess(char ** arrs, int dictionaryLength){
	int length = 0;
	int size = dictionaryLength * INIT_HASHTABLE_SIZE_RELATE;

	HashTable * hashTable = createHashTable(size);
	size = hashTable->size;
	int halfsize = size / 2;

	char * word = arrs[0];
	while (length < dictionaryLength){
		word = arrs[length++];
		switch (addHash(hashTable, word)){
		case ADDHASH_ERROR:
			free(hashTable->arrs);
			free(hashTable);
			return NULL;
		case ADDHASH_NEW:
			hashTable->used++;
			break;
		case ADDHASH_COLLISION:
			hashTable->collision++;
		}

		if (hashTable->used >= halfsize){
			HashTable * hashTable2 = rehash(hashTable);
			if (hashTable2 == NULL){
				return NULL;
			}
			else{
				hashTable = hashTable2;
				size = hashTable->size;
				halfsize = size / 2;
			}
		}
	}

	return hashTable;
}

HashTable * DeletionHashTableProcess(char ** arrs, int dictionaryLength){
	int length = 0;
	int size = dictionaryLength * INIT_HASHTABLE_SIZE_RELATE;

	HashTable * hashTable = createHashTable(size);
	size = hashTable->size;
	int halfsize = size / 2;

	char * word = arrs[0];
	while (length < dictionaryLength){
		word = arrs[length++];
		int length = countWordLength(word);
		for (int i = 0; i < length; i++){
			switch (addHash(hashTable, recreateWordWithoutIndex(word, i))){
			case ADDHASH_ERROR:
				free(hashTable->arrs);
				free(hashTable);
				return NULL;
			case ADDHASH_NEW:
				hashTable->used++;
				break;
			case ADDHASH_COLLISION:
				hashTable->collision++;
			}

			if (hashTable->used >= halfsize){
				HashTable * hashTable2 = rehash(hashTable);
				if (hashTable2 == NULL){
					return NULL;
				}
				else{
					hashTable = hashTable2;
					size = hashTable->size;
					halfsize = size / 2;
				}
			}
		}
	}

	return hashTable;
}

int compareWord(char * c1, char * c2){
	while (*c1 != '\0' || *c2 != '\0'){
		if (*c1 != *c2)
			return FALSE;
		c1++;
		c2++;
	}

	return TRUE;
}

int isWordExists(HashTable * hashTable, char * word){
	int hashInt = hash(word, hashTable->size);
	HashTableNode * now = hashTable->arrs[hashInt];
	while (now != NULL){
		if (compareWord(now->word, word))
			return TRUE;
		now = now->next;
	}
	return FALSE;
}

void printListWordDeletion(HashTable * hashTable, char * word){
	int length = countWordLength(word);
	for (int i = 0; i < length; i++){
		int hashInt = hash(recreateWordWithoutIndex(word, i), hashTable->size);
		HashTableNode * hashNode = hashTable->arrs[hashInt];
		while (hashNode != NULL){
			if (countWordLength(hashNode->word) == length){
				printf("%s ", hashNode->word);
			}
			hashNode = hashNode->next;
		}
	}
}

int main(int argc, char ** argv){
	char filepath[] = "D:\\work\\mine\\class\\lab\\data_structure\\all\\Hash\\full.txt";
	readFileReturn * ret = readFile(filepath);
	if (ret == NULL){
		printf("Failed to read file %s", filepath);
	}
	printf("Successfully create WordList\nTotal size : %d word(s)\n", ret->size);

	HashTable * hashTable = HashTableProcess(ret->arrs, ret->size);
	printf("\n=======================================\n\nSuccessfully create HashTable\nHashTable size: %d\nCollision: %d\nUsed: %d\n", hashTable->size, hashTable->collision, hashTable->used);

	//HashTable * hashTableDeletion = DeletionHashTableProcess(ret->arrs, ret->size);
	//printf("\n=======================================\n\nSuccessfully create DeletionHashTable\nHashTable size: %d\nCollision: %d\nUsed: %d\n", hashTable->size, hashTable->collision, hashTable->used);

	char c[100] = "word";
	while(true) {
		printf("\n=======================================\n\nEnter the word: ");
		scanf("%s", c);

		printf("Word hash is %d\n", hash(c, hashTable->size));

		if (compareWord("SYSTEM:END", c)) break;

		if (isWordExists(hashTable, c) == TRUE){
			printf("Word found\n");
		}
		else{
			printf("Word not found\n");
		}
	};
	_getch();
	return 0;
}