#include <stdio.h>
#include <stdlib.h>
typedef struct HuffmanNode{
    unsigned char symbol;
    unsigned int freq;
    struct HuffmanNode *left, *right, *parent;
} HuffmanNode;
#include "minHeap.h"

HuffmanNode* getNode(unsigned char symbol, unsigned int freq, HuffmanNode* left, HuffmanNode* right, HuffmanNode* parent) {
    HuffmanNode* node = malloc(sizeof(HuffmanNode));
    node->symbol = symbol;
    node->freq = freq;
    node->left = left;
    node->right = right;
    node->parent = parent;
    return node;
}
void printWords(unsigned int counter[]) {
    for (int i = 0; i < 256; i++) {
        if (counter[i] == 0) continue;
        printf("%c(%d):%d, ", i, i, counter[i]);
    }
    printf("\n");
}

char* fgetString(FILE* input) {
    if (input == NULL) {
        perror("File error");
        return NULL;
    }
    int capacity = 16, count = 0;
    char c;
    char* str = malloc(capacity * sizeof(char));
    while ((c = fgetc(input)) != EOF) {
        str[count++] = c;
        if (count == capacity - 1) {
            capacity *= 2;
            char* newStr = realloc(str, capacity * sizeof(char));
            if (newStr == NULL) {
                perror("Realloc Failed");
                free(str);
                return NULL;
            }
            str = newStr;
        }
    }
    str[count] = '\0';
    char* newStr = realloc(str, (count + 1) * sizeof(char));
    if (newStr != NULL)
        str = newStr;
    return str;
}

unsigned int* countFrequencies(FILE* input) {
    if (input == NULL) {
        perror("File error");
        return NULL;
    }
    unsigned int* res = calloc(256, sizeof(int));
    char c;
    while ((c = fgetc(input)) != EOF) {
        if (c >= 256) continue;
        res[c]++;
    }
    return res;
}


HuffmanNode* constructHuffmanTree(unsigned int counter[]) {
    PriorityQueue q;
    initPQ(&q, 256);
    for (int i = 0; i < 256; i++) {
        if (counter[i] == 0) continue;
        HuffmanNode* node = getNode(i, counter[i], NULL, NULL, NULL);
        insertPQ(&q, node);
    }

    while (q.size > 1) {
        HuffmanNode* left = popPQ(&q);
        HuffmanNode* right = popPQ(&q);
        HuffmanNode* parent = getNode(0, left->freq + right->freq, left, right, NULL);
        left->parent = parent;
        right->parent = parent;
        insertPQ(&q, parent);
    }
    HuffmanNode* root = popPQ(&q);
    return root;
}

void generateCodesHelper(HuffmanNode* root, char currentPath[], int depth, char** table) {
    char* huffmanCodes[256];
    if (!root->left && !root->right) {
        currentPath[depth] = '\0';
        table[root->symbol] = strdup(currentPath);
        return;
    }
    if (root->left) {
        currentPath[depth] = '0';
        generateCodesHelper(root->left, strcat(currentPath, "0"), depth + 1, table);
    }
    if (root->right) {
        currentPath[depth] = '1';
        generateCodesHelper(root->right, strcat(currentPath, "1"), depth + 1, table);
    }
}

char** generateCodes(HuffmanNode* root) {
    char** table = (char**)malloc(256 * sizeof(char*));
    for (int i = 0; i < 256; i++) table[i] = NULL;
    char currentPath[256];
    generateCodesHelper(root, currentPath, 0, table);
    return table;
}
void freeCodes(char** codes) {
    for (int i = 0; i < 256; i++)
        free(codes[i]);
    free(codes);
}
void printHuffmanNode(const HuffmanNode* node) {
    printf("%c%d, ", node->symbol, node->freq);
}

void printHuffmanTreeInorder(HuffmanNode* root) {
    if (!root) return;
    printHuffmanTreeInorder(root->left);
    printHuffmanNode(root);
    printHuffmanTreeInorder(root->right);
}

void printHuffmanTreePreorder(HuffmanNode* root) {
    if (!root) return;
    printHuffmanNode(root);
    printHuffmanTreePreorder(root->left);
    printHuffmanTreePreorder(root->right);
}

void freeHuffmanTree(HuffmanNode* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

void testFunkcija() {
    FILE* input = fopen("tekst.txt", "r");
    unsigned int* counter = countFrequencies(input);
    fseek(input, 0, SEEK_SET);

    HuffmanNode* root = constructHuffmanTree(counter);
    char** codes = generateCodes(root);
    printf("Counter: ");
    for (int i = 0; i < 256; i++) {
        if (counter[i] == 0) continue;
        printf("%c(%d), ", i, counter[i]);
    }
    printf("\nKodovi: ");
    for (int i = 0; i < 256; i++) {
        if (codes[i] == NULL) continue;
        printf("%c(%s), ", i, codes[i]);
    }
    free(counter);
    fclose(input);
    freeHuffmanTree(root);
    freeCodes(codes);
}