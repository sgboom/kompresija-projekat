#include "../../include/huffman.h"

HuffmanNode* getNode(unsigned char symbol, unsigned int freq, HuffmanNode* left, HuffmanNode* right) {
    HuffmanNode* node = malloc(sizeof(HuffmanNode));
    node->symbol = symbol;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}
void printWords(unsigned int counter[]) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
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
    int c;
    char* str = malloc(capacity * sizeof(char));
    while ((c = fgetc(input)) != EOF) {
        str[count++] = (char)c;
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
    unsigned int* res = calloc(ALPHABET_SIZE, sizeof(int));
    int c;
    while ((c = fgetc(input)) != EOF) {
        if (c >= ALPHABET_SIZE) continue;
        res[c]++;
    }
    return res;
}


HuffmanNode* constructHuffmanTree(unsigned int counter[]) {
    if (!counter) return NULL;
    PriorityQueue q;
    initPQ(&q, ALPHABET_SIZE);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (counter[i] == 0) continue;
        HuffmanNode* node = getNode(i, counter[i], NULL, NULL);
        insertPQ(&q, node);
    }

    while (q.size > 1) {
        HuffmanNode* left = popPQ(&q);
        HuffmanNode* right = popPQ(&q);
        HuffmanNode* parent = getNode(0, left->freq + right->freq, left, right);
        insertPQ(&q, parent);
    }
    HuffmanNode* root = popPQ(&q);
    return root;
}

void generateCodesHelper(const HuffmanNode* root, char currentPath[], int depth, char** table) {
    if (!table || !root) return;
    if (!root->left && !root->right) {
        currentPath[depth] = '\0';
        table[root->symbol] = strdup(currentPath);
        return;
    }
    if (root->left) {
        currentPath[depth] = '0';
        generateCodesHelper(root->left, currentPath, depth + 1, table);
    }
    if (root->right) {
        currentPath[depth] = '1';
        generateCodesHelper(root->right, currentPath, depth + 1, table);
    }
}

char** generateCodes(const HuffmanNode* root) {
    char** table = (char**)malloc(ALPHABET_SIZE * sizeof(char*));
    for (int i = 0; i < ALPHABET_SIZE; i++) table[i] = NULL;
    char currentPath[ALPHABET_SIZE];
    generateCodesHelper(root, currentPath, 0, table);
    return table;
}
void freeCodes(char** codes) {
    if (!codes) return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
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
unsigned long fileByteSize(FILE* inputFile) {
    unsigned long bytesCount = 0;
    while (fgetc(inputFile) != EOF) bytesCount++;
    return bytesCount;
}
void staticCompressFile(const char* inputFileName, const char* outputFileName) {
    FILE* inputFile = fopen(inputFileName, "rb");
    if (!inputFile) { return; }

    unsigned int* freqTable = countFrequencies(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    HuffmanNode* root = constructHuffmanTree(freqTable);
    if (!root) {
        fclose(inputFile);
        free(freqTable);
        return;
    }
    char** codes = generateCodes(root);

    FILE* outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        fclose(inputFile);
        free(freqTable);
        freeHuffmanTree(root);
        freeCodes(codes);
        return;
    }

    fwrite(freqTable, sizeof(unsigned int), ALPHABET_SIZE, outputFile);

    bitWriter* b = initBitWriter(outputFile, 1024);

    if (!b) {
        fclose(inputFile);
        free(freqTable);
        freeHuffmanTree(root);
        freeCodes(codes);
        fclose(outputFile);
        return;
    }

    int c;
    while ((c = fgetc(inputFile)) != EOF) {
        char* byteCode = codes[c];
        for (char* p = byteCode; *p != '\0'; p++) {
            addBits(b, *p - '0', 1);
        }
    }

    fclose(inputFile);
    free(freqTable);
    freeHuffmanTree(root);
    freeCodes(codes);
    closeBitWriter(b);
    freeBitWriter(b);
    fclose(outputFile);
}

void staticDecompressFile(const char* compressedFilename, const char* outputFilename) {
    FILE* file = fopen(compressedFilename, "rb");
    if (!file) return;

    unsigned int freq[ALPHABET_SIZE];
    fread(freq, sizeof(unsigned int), ALPHABET_SIZE, file);
    HuffmanNode* root = constructHuffmanTree(freq);
    if (!root) {
        fclose(file);
        return;
    }
    bitReader* r = initBitReader(file, 1024);
    if (!r) {
        freeHuffmanTree(root);
        fclose(file);
        return;
    }
    HuffmanNode* current = root;
    FILE* writer = fopen(outputFilename, "wb");
    if (!writer) {
        freeBitReader(r);
        freeHuffmanTree(root);
        fclose(file);
        return;
    }
    int bit;
    while ((bit = getBit(r)) != -1) {
        if (bit == 0) current = current->left;
        else current = current->right;

        if (current->left == NULL && current->right == NULL) {
            fputc(current->symbol, writer);
            current = root;
        }
    }
    freeBitReader(r);
    freeHuffmanTree(root);
    fclose(writer);
    fclose(file);
}