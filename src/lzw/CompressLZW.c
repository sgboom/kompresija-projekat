#include <stdio.h>
#include <stdlib.h>
#include "../../include/lzw.h"
#include "../../include/bitFile.h"

void INSERT(HashTable *ht, short int prefix_code, char character, short int code) {
    if (ht == NULL) {
        return;
    }
    unsigned int index = (prefix_code * 256 + (unsigned char)character + TABLE_SIZE) % TABLE_SIZE;
    Node *new = malloc(sizeof(Node));
    new->prefix_code = prefix_code;
    new->code = code;
    new->character = character;
    new->next = NULL;

    Node *temp = ht->niz[index];
    if (temp == NULL) {
        ht->niz[index] = new;
    }
    else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new;
    }
}

void FreeHashTable(HashTable *ht) {
    if (ht == NULL) {
        return;
    }
    for (unsigned short int i = 0; i < TABLE_SIZE; i++) {
        Node *temp = ht->niz[i];
        while (temp != NULL) {
            Node *next = temp->next;
            free(temp);
            temp = next;
        }
        ht->niz[i] = NULL;
    }
    free(ht);
}

short int FIND_CODE(const HashTable *ht, short int prefix_code, char character) {
    if (ht == NULL) {
        exit(-1);
    }
    unsigned int index = (prefix_code * 256 + (unsigned char)character + TABLE_SIZE) % TABLE_SIZE;
    const Node *temp = ht->niz[index];
    while (temp != NULL) {
        if (temp->prefix_code == prefix_code && temp->character == character) {
            return temp->code;
        }
        temp = temp->next;
    }
    return -1;
}

short int Add_Chars(HashTable *ht) {
    short int code = 0;
    for (unsigned short int i = 0; i < 256; i++) {
        INSERT(ht, -1, ((char)i), code++);
    }
    return code;
}

int LZW_Encode(const wchar_t* filenameIn,const wchar_t* filenameOut) {
	HashTable *ht = InitHashTable();
    short int code = Add_Chars(ht);
    
    if (ht == NULL || filenameIn == NULL || filenameOut == NULL) {
        return 1;
    }
    short int code_length=8;
    FILE *f = _wfopen(filenameOut, L"wb");
    FILE *read=_wfopen(filenameIn, L"rb");
    
    bitWriter *writer = initBitWriter(f,1024);
    int c=fgetc(read);
    short int prefix = FIND_CODE(ht, -1, (char)c);
    char ret = 0;
    while ((c=fgetc(read))!=-1) {
    	if(c == 0){
    		ret = 1;
    		break;
		};
        short int temp = FIND_CODE(ht, prefix, (char)c);
        if (temp != -1) {
            prefix = temp;
        }
        else {
            addBits(writer,prefix,code_length);
            if ((1<<code_length)==code) code_length++;
            if (code!=TABLE_SIZE-1) {
                INSERT(ht, prefix, (char)c, code++);
            }
            prefix = FIND_CODE(ht, -1, (char)c);
        }
    }
    if (prefix != -1) {
        addBits(writer,prefix,code_length);
    }
    closeBitWriter(writer);
    freeBitWriter(writer);
    FreeHashTable(ht);
    fclose(f);
    fclose(read);
    return ret;
}

HashTable* InitHashTable() {
    HashTable *ht = calloc(1, sizeof(HashTable));
    return ht;
}

