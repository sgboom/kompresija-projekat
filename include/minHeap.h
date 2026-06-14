#ifndef MINHEAP_H
#define MINHEAP_H
#include "huffman_common.h"
// ---------- POMOCNE FUNKCIJE ----------

int parent(int i);
int leftChild(int i);
int rightChild(int i);
void swap(HuffmanNode** a, HuffmanNode** b);

// ---------- POMOCNE FUNKCIJE ----------
void initPQ(PriorityQueue* q, int capacity);
HuffmanNode* topMinPQ(const PriorityQueue* q);
void shiftUp(const PriorityQueue* q, int i);
void shiftDown(PriorityQueue* q, int i);
void insertPQ(PriorityQueue* q, HuffmanNode* node);
HuffmanNode* popPQ(PriorityQueue* q);
bool emptyPQ(const PriorityQueue* q);
bool isFullPQ(const PriorityQueue* q);
void freePQ(PriorityQueue* q);

#endif