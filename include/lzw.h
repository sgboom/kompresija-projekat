#ifndef LZW_H
#define LZW_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define length 4096
#define N 4095

// CompressLZW

typedef struct Node {
    struct Node *next;
    short int prefix_code;
    short int code;
    char character;
} Node;

typedef struct {
    Node *niz[length];
} HashTable;

void WriteBits(FILE *out, short int code_length, short int code);
void FlushBits(FILE *out);
void INSERT(HashTable *ht, short int prefix_code, char character, short int code);
short int FIND_CODE(const HashTable *ht, short int prefix_code, char character);
short int Add_Chars(HashTable *ht);
void LZW_Encode(HashTable *ht, char filename[], short int code);
HashTable* InitHashTable();
void freehashtable(HashTable *ht);

// DecompressLZW

void FREE_TABLE(char* table[],int codes);
int ReadBits(FILE* f, int len);
void ADD_CHARS(char* table[]);
void ADD_STRING(char* table[],char str[],int code);
void LZW_DECODE(char* table[],char filename_read[],char filename_write[]);
void FREE_TABLE(char* table[],int codes);

#endif