#include "../../include/hashTable.h"
void insert(HashTable *ht,unsigned short int prefix_code,char *string) {
    unsigned short int indeks=hashFunc(string)%length;
    Node *temp=ht->niz[indeks];
    if (temp==NULL) {
        ht->niz[indeks]=(malloc(sizeof(Node))) ;
        ht->niz[indeks]->next=NULL;
        ht->niz[indeks]->prefix_code=prefix_code;
        ht->niz[indeks]->character=string;
        return;
    }
    while(temp->next!=NULL) {
        temp=temp->next;
    }
    temp->next=(malloc(sizeof(Node)));
    temp->next->next=NULL;
    temp->next->prefix_code=prefix_code;
    temp->next->character=string;
}
unsigned int hashFunc(const char *str) {
    unsigned int hash=0;
    while (*str!='\0') {
        hash+=*str;
        hash+=hash<<10;
        hash^=hash>>6;
        str++;
    }
    hash+=hash<<3;
    hash^=hash>>11;
    hash+=hash<<15;
    return hash;
}
unsigned short int find_code(HashTable ht, const char *string) {
    unsigned short int indeks=hashFunc(string)%length;
    Node *temp=ht.niz[indeks];
    while(temp!=NULL) {
        if (temp->character==string) {
            return temp->prefix_code;
        }
        temp=temp->next;
    }
    return -1;
}
HashTable* InitHasTable() {
    HashTable* hash=(HashTable*)calloc(1,sizeof(HashTable));
    return hash;
}
void freeHashTable(HashTable *ht) {
    for (unsigned short int i=0;i<length;i++) {
        Node *temp=ht->niz[i];
        while (temp!=NULL) {
            Node *next=temp->next;
            free(temp);
            temp=next;
        }
        ht->niz[i]=NULL;
    }
    free(ht);
}