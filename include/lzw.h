#ifndef LZW_H
#define LZW_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 4096
#define N 4095

// CompressLZW

typedef struct Node {
    struct Node *next;
    short int prefix_code;
    short int code;
    unsigned char character;
} Node;

typedef struct {
    Node *niz[TABLE_SIZE];
} HashTable;

void WriteBits(FILE *out, short int code_length, short int code);
void FlushBits(FILE *out);
void INSERT(HashTable *ht, short int prefix_code,char character, short int code);
short int FIND_CODE(const HashTable *ht, short int prefix_code, char character);
short int Add_Chars(HashTable *ht);
int LZW_Encode(const wchar_t *filenameIn,const wchar_t *filenameOut);
HashTable* InitHashTable();
void freehashtable(HashTable *ht);

// DecompressLZW
typedef struct {
	char* string;
	unsigned short length;
} decompressElement;


int ReadBits(FILE* f, int len);
void ADD_CHARS(decompressElement table[]);
//void ADD_STRING(char* table[],char str[],int code);
void LZW_DECODE(const wchar_t* filename_read,const wchar_t* filename_write);
void FREE_TABLE(decompressElement table[],int codes);

#endif
