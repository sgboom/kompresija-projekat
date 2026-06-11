
#include <stdio.h>
#include <stdlib.h>
#define length 4096

unsigned long buffer = 0;
int bit_count = 0;

typedef struct Node {
    struct Node *next;
    short int prefix_code;
    short int code;
    char character;
} Node;

typedef struct {
    Node *niz[length];
} HashTable;

void WriteBits(FILE *out, short int code_length, short int code) {
    for ( int i = code_length - 1; i >= 0; i--) {
        int bit = (code >> i) & 1;
        buffer = (buffer << 1) | bit;
        bit_count++;
        if (bit_count == 8) {
            fputc((int)buffer, out);
            bit_count = 0;
            buffer = 0;
        }
    }
}
void FlushBits(FILE *out) {
    if (bit_count>0) {
        buffer=buffer<<(8-bit_count);
        fputc((int)buffer, out);
        bit_count=0;
        buffer=0;
    }
}
void INSERT(HashTable *ht, short int prefix_code, char character, short int code) {
    if (ht == NULL) {
        return;
    }
    unsigned int index = (prefix_code * 256 + (unsigned char)character) % length;
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

short int FIND_CODE(const HashTable *ht, short int prefix_code, char character) {
    if (ht == NULL) {
        exit(-1);
    }
    unsigned int index = (prefix_code * 256 + (unsigned char)character) % length;
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
        if ((FIND_CODE(ht, 0, ((char)i))) == -1) {
            INSERT(ht, 0, ((char)i), code++);
        }
    }
    return code;
}

void LZW_Encode(HashTable *ht, char filename[], short int code) {
    if (ht == NULL || filename == NULL) {
        return;
    }
    short int code_length=8;
    FILE *f = fopen("coded.txt", "wb");
    FILE *read=fopen(filename, "r");
    int c=fgetc(read);
    short int prefix = FIND_CODE(ht, 0, (char)c);
    while ((c=fgetc(read))!=-1) {
        short int temp = FIND_CODE(ht, prefix, (char)c);
        if (temp != -1) {
            prefix = temp;
        }
        else {
            WriteBits(f,code_length,prefix);
            if ((1<<code_length)==code) code_length++;
            if (code!=length-1) {
                INSERT(ht, prefix, (char)c, code++);
            }
            prefix = FIND_CODE(ht, 0, (char)c);
        }
    }
    if (prefix != -1) {
        //printf("%hd ", prefix);
        WriteBits(f,code_length,prefix);
    }
    FlushBits(f);
    fclose(f);
    fclose(read);
}

HashTable* InitHashTable() {
    HashTable *ht = calloc(1, sizeof(HashTable));
    return ht;
}

void FreeHashTable(HashTable *ht) {
    if (ht == NULL) {
        return;
    }
    for (unsigned short int i = 0; i < length; i++) {
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

int main() {
    HashTable *ht = InitHashTable();
    short int coding = Add_Chars(ht);
    LZW_Encode(ht, "prica.txt", coding);
    FreeHashTable(ht);
    return 0;
}