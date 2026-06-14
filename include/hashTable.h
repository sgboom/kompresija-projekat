#ifndef HASHTABLE_H
#define HASHTABLE_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define length 4096

typedef struct Node {
    struct Node *next;
    unsigned short int prefix_code;
    char *character;
}Node;
typedef struct {
    Node *niz[length];
}HashTable;

unsigned int hashFunc(const char* str);
void insert(HashTable *ht,unsigned short int prefix_code,char *string);
unsigned int hashFunc(const char *str);
unsigned short int find_code(HashTable ht, const char *string);
HashTable* InitHasTable();
void freeHashTable(HashTable *ht);

#endif