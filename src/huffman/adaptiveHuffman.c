#include "../../include/huffman.h"

AdaptiveHuffmanNode* createNode(const int value, const int weight) {
    AdaptiveHuffmanNode* node = malloc(sizeof(AdaptiveHuffmanNode));
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->value = value;
    node->weight = weight;
    node->id = -1;
    return node;
}

void initTree(AdaptiveHuffmanHeader* h) {
    h->nyt = createNode(-2, 0);
    h->root = h->nyt;
    h->nyt->id = MAX_NODES - 1;
    memset(h->nodeList, 0, sizeof(h->nodeList));
    h->nodeList[h->nyt->id] = h->nyt;
    h->nodeCount = 1;
    memset(h->seenChars, 0, sizeof(h->seenChars));
}

void swapInt(int* value1, int* value2) {
    const int temp = *value1;
    *value1 = *value2;
    *value2 = temp;
}

void swapNodes(AdaptiveHuffmanHeader* h, AdaptiveHuffmanNode* node1, AdaptiveHuffmanNode* node2) {
    if (!node1 || !node2 || node1 == node2 || node1->parent == node2 || node2->parent == node1) return;

    AdaptiveHuffmanNode* parent1 = node1->parent, *parent2 = node2->parent;

    if (parent1->left == node1) parent1->left = node2;
    else parent1->right = node2;

    if (parent2->left == node2) parent2->left = node1;
    else parent2->right = node1;

    node1->parent = parent2;
    node2->parent = parent1;

    swapInt(&node1->id, &node2->id);

    h->nodeList[node1->id] = node1;
    h->nodeList[node2->id] = node2;
}

AdaptiveHuffmanNode* findMaxInBlock(const AdaptiveHuffmanHeader* h, const AdaptiveHuffmanNode* node) {
    int id = node->id;
    while (id + 1 < MAX_NODES && h->nodeList[id + 1] && h->nodeList[id + 1]->weight == node->weight) {
        id++;
    }
    return h->nodeList[id];
}

void updateAH(AdaptiveHuffmanHeader* h, const int c) {
    AdaptiveHuffmanNode* curr = h->seenChars[c];
    if (curr == NULL) {
        AdaptiveHuffmanNode* oldNyt = h->nyt;

        AdaptiveHuffmanNode* newNyt = createNode(-2, 0);
        AdaptiveHuffmanNode* newLeaf = createNode(c, 1);

        oldNyt->left = newNyt;
        oldNyt->right = newLeaf;
        newNyt->parent = oldNyt;
        newLeaf->parent = oldNyt;

        int currIndx = oldNyt->id;

        newLeaf->id = currIndx - 1;
        h->nodeList[newLeaf->id] = newLeaf;

        newNyt->id = currIndx - 2;
        h->nodeList[newNyt->id] = newNyt;

        h->seenChars[c] = newLeaf;
        h->nyt = newNyt;

        curr = oldNyt;
    }
    while (curr != NULL) {
        AdaptiveHuffmanNode* maxNode = findMaxInBlock(h, curr);
        if (maxNode != curr && maxNode != curr->parent) {
            swapNodes(h, maxNode, curr);
        }
        curr->weight++;
        curr = curr->parent;
    }
}

void writeNodeCode(bitWriter* writer, AdaptiveHuffmanNode* node) {
    int codes[512], count = 0;
    AdaptiveHuffmanNode* curr = node;
    while (curr->parent != NULL) {
        if (curr->parent->left == curr) {
            codes[count++] = 0;
        }
        else {
            codes[count++] = 1;
        }
        curr = curr->parent;
    }
    for (int i = count - 1; i >= 0; i--) {
        addBits(writer, codes[i], 1);
    }
}

void adaptiveCompressFile(const wchar_t *inputFilename, const wchar_t *outputFilename) {
    FILE* file = _wfopen(inputFilename, L"rb");
    if (!file) return;
    FILE* outFile = _wfopen(outputFilename, L"wb");
    bitWriter* writer = initBitWriter(outFile, 1024);
    AdaptiveHuffmanHeader h;
    initTree(&h);
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (h.seenChars[c] != NULL) {
            writeNodeCode(writer, h.seenChars[c]);
        }
        else {
            if (h.nyt != h.root) writeNodeCode(writer, h.nyt);
            addBits(writer, c, 8);
        }
        updateAH(&h, c);
    }
    closeBitWriter(writer);
    freeBitWriter(writer);
    fclose(file);
    fclose(outFile);
    freeAdaptiveHuffman(&h);
}

int read8Bits(bitReader *reader) {
    int result = 0;
    for (int i = 0; i < 8; i++) {
        int bit = getBit(reader);
        if (bit == -1) return -1;
        result = (result << 1) | bit;
    }
    return result;
}

void adaptiveDecompressFile(const wchar_t* compressedFilename, const wchar_t* outputFilename) {
    FILE* binFile = _wfopen(compressedFilename, L"rb");
    if (!binFile) {
        return;
    }
    bitReader* reader = initBitReader(binFile, 1024);
    if (!reader) {
        fclose(binFile);
        return;
    }
    FILE* file = _wfopen(outputFilename, L"wb");
    if (!file) {
        fclose(binFile);
        return;
    }
    AdaptiveHuffmanHeader h;
    initTree(&h);

    AdaptiveHuffmanNode* curr = h.root;
    int bit;

    while ((bit = getBit(reader)) != -1) {
        if (h.root == h.nyt) {
            unsigned char first_char = bit;
            for (int i = 0; i < 7; i++) {
                first_char = (first_char << 1) | getBit(reader);
            }
            fputc(first_char, file);
            updateAH(&h, first_char);
            curr = h.root;
            continue;
        }

        if (bit == 0) curr = curr->left;
        else curr = curr->right;

        if (curr->left == NULL && curr->right == NULL) {
            if (curr == h.nyt) {
                int new_char = 0;
                int valid = 1;
                for (int i = 0; i < 8; i++) {
                    int next_bit = getBit(reader);
                    if (next_bit == -1) {
                        valid = 0;
                        break;
                    }
                    new_char = (new_char << 1) | next_bit;
                }
                if (!valid) break;

                fputc(new_char, file);
                updateAH(&h, new_char);
            } else {
                fputc(curr->value, file);
                updateAH(&h, curr->value);
            }
            curr = h.root;
        }
    }
    freeBitReader(reader);
    fclose(binFile);
    fclose(file);
    freeAdaptiveHuffman(&h);

}
void freeAdaptiveHuffman(AdaptiveHuffmanHeader* h) {
    if (!h) return;
    for (int i = 0; i < MAX_NODES; i++) {
        if (h->nodeList[i]) {
            free(h->nodeList[i]);
            h->nodeList[i] = NULL;
        }
    }
    h->root = NULL;
    h->nyt = NULL;
    h->nodeCount = 0;
    memset(h->seenChars, 0, sizeof(h->seenChars));
}