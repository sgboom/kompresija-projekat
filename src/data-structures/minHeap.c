#include "../../include/minHeap.h"
// ---------- POMOCNE FUNKCIJE ----------
int parent(int i) { return (i - 1) / 2; }

int leftChild(int i) {return 2 * i + 1; }

int rightChild(int i) {return 2 * i + 2;}

void swap(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* temp = *a;
    *a = *b;
    *b = temp;
}
// ---------- POMOCNE FUNKCIJE ----------
void initPQ(PriorityQueue* q, int capacity) {
    q->size = 0;
    q->capacity = capacity;
    q->data = malloc(capacity * sizeof(HuffmanNode*));
}

HuffmanNode* topMinPQ(const PriorityQueue* q) {
    if (q->size > 0) return NULL;
    return q->data[0];
}
void shiftUp(const PriorityQueue* q, int i) {
    while (i > 0 && q->data[parent(i)]->freq > q->data[i]->freq) {
        swap(&q->data[parent(i)], &q->data[i]);
        i = parent(i);
    }
}

void shiftDown(PriorityQueue* q, int i) {
    int left = leftChild(i), right = rightChild(i), minPos = i;
    if (left < q->size && q->data[left]->freq < q->data[minPos]->freq) minPos = left;
    if (right < q->size && q->data[right]->freq < q->data[minPos]->freq) minPos = right;
    if (i != minPos) {
        swap(&q->data[i], &q->data[minPos]);
        shiftDown(q, minPos);
    }
}

void insertPQ(PriorityQueue* q, HuffmanNode* node) {
    if (q->size == q->capacity) return;
    q->data[q->size++] = node;
    shiftUp(q, q->size - 1);
}

HuffmanNode* popPQ(PriorityQueue* q) {
    if (q->size <= 0) return NULL;
    q->size--;
    HuffmanNode* result = q->data[0];
    q->data[0] = q->data[q->size];
    shiftDown(q, 0);
    return result;
}

bool emptyPQ(const PriorityQueue* q) {
    return q->size == 0;
}

bool isFullPQ(const PriorityQueue* q) {
    return q->size == q->capacity;
}

void freePQ(PriorityQueue* q) {
    free(q->data);
    q->data = NULL;
    q->size = 0;
    q->capacity = 0;
}