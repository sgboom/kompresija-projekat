#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define length 10
typedef struct Node{
    struct Node *next;
    unsigned int key;
    char* value;
}Node;
typedef struct{
      Node *hash[length];
}hashtable;
unsigned int hashFunc(char* str) {
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
void insert(char* string,hashtable* hash) {
    int indeks=hashFunc(string)%length;
    printf("indeks=%d\n",indeks);
     Node *temp=hash->hash[indeks];
    if (temp==NULL) {
        hash->hash[indeks]=malloc(sizeof(Node));
        hash->hash[indeks]->key=hashFunc(string);
        hash->hash[indeks]->value=string;
        hash->hash[indeks]->next=NULL;
        return;
    }
    while (temp->next!=NULL) {
        temp=temp->next;
    }
    temp->next=malloc(sizeof(Node));
    temp->next->next=NULL;
    temp->next->value=string;
    temp->next->key=hashFunc(string);
}
char* find(char* string,hashtable* hash) {
    int strHash=hashFunc(string);
    int indeks=strHash%length;
    Node *temp=hash->hash[indeks];
    while (temp->next!=NULL) {
        if (strHash==temp->key) {
            return temp->value;
        }
        temp=temp->next;
    }
    return NULL;

}
void delete(Node* head){
    while (head!=NULL) {
        Node *temp=head;
        head=head->next;
        free(temp);
    }
}
void freehashtable(hashtable* hash) {
    for (int i=0;i<length;i++) {
        if (hash->hash[i]!=NULL) {
            delete(hash->hash[i]);
        }
    }
}
hashtable* inithashtable() {
    hashtable* hash=(hashtable*)calloc(1,sizeof(hashtable));
    return hash;
}
void printhashtable(hashtable* hash) {
    for (int i=0;i<length;i++) {
        Node *temp=hash->hash[i];
        if (temp!=NULL) {
            printf("%d=%s\n",hash->hash[i]->key,hash->hash[i]->value);
            while (temp->next!=NULL) {
                printf("%d=%s\n",hash->hash[i]->key,hash->hash[i]->value);
                temp=temp->next;
            }
        }
    }
}
int main() {
    hashtable* hash=inithashtable();
    char str[] = "Hello World!";
    insert(str,hash);
    insert("ganci",hash);
    printhashtable(hash);
}
