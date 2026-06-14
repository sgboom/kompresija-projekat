#ifndef HUFFMAN_COMMON_H
#define HUFFMAN_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// =========================================================================
// 1. STATIČKI HUFFMAN - STRUKTURE I MAKROI
// =========================================================================
#define ALPHABET_SIZE 256

typedef struct HuffmanNode {
    unsigned char symbol;
    unsigned int freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

typedef struct PriorityQueue{
    int size;
    int capacity;
    HuffmanNode** data;
} PriorityQueue;

// =========================================================================
// 2. DINAMIČKI (ADAPTIVNI) HUFFMAN - STRUKTURE I MAKROI
// =========================================================================
#define MAX_NODES 513

typedef struct adaptiveHuffmanNode {
    int value, weight, id;
    struct adaptiveHuffmanNode *left, *right, *parent;
} AdaptiveHuffmanNode;

typedef struct adaptiveHuffmanHeader {
    AdaptiveHuffmanNode *root, *nyt;
    AdaptiveHuffmanNode *seenChars[ALPHABET_SIZE];
    AdaptiveHuffmanNode *nodeList[MAX_NODES];
    int nodeCount;
} AdaptiveHuffmanHeader;

#endif