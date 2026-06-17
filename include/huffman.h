#ifndef HUFFMAN_H
#define HUFFMAN_H


#include <wchar.h>
#include "huffman_common.h"
#include "minHeap.h"
#include "bitFile.h"

//staitcki huffman
HuffmanNode* getNode(unsigned char symbol, unsigned int freq, HuffmanNode* left, HuffmanNode* right);
void printWords(unsigned int counter[]);
unsigned int* countFrequencies(FILE* input);
HuffmanNode* constructHuffmanTree(unsigned int counter[]);
void generateCodesHelper(const HuffmanNode* root, char currentPath[], int depth, char** table);
char** generateCodes(const HuffmanNode* root);
void freeCodes(char** codes);
void printHuffmanNode(const HuffmanNode* node);
void printHuffmanTreeInorder(HuffmanNode* root);
void printHuffmanTreePreorder(HuffmanNode* root);
void freeHuffmanTree(HuffmanNode* root);
unsigned long fileByteSize(FILE* inputFile);
void writeHeader(char* fileName, unsigned int* freq);
void staticCompressFile(const wchar_t* inputFileName, const wchar_t* outputFileName);
void staticDecompressFile(const wchar_t* compressedFilename, const wchar_t* outputFilename);
void testFunkcija();

//dinamicki huffman

AdaptiveHuffmanNode* createNode(int value, int weight);
void initTree(AdaptiveHuffmanHeader* h);
void swapInt(int* value1, int* value2);
void swapNodes(AdaptiveHuffmanHeader* h, AdaptiveHuffmanNode* node1, AdaptiveHuffmanNode* node2);
AdaptiveHuffmanNode* findMaxInBlock(const AdaptiveHuffmanHeader* h, const AdaptiveHuffmanNode* node);
void updateAH(AdaptiveHuffmanHeader* h, int c);
void writeNodeCode(bitWriter* writer, AdaptiveHuffmanNode* node);

void adaptiveCompressFile(const wchar_t *inputFilename, const wchar_t *outputFilename);
int read8Bits(bitReader *reader);
void adaptiveDecompressFile(const wchar_t* compressedFilename, const wchar_t* outputFilename);
void freeAdaptiveHuffman(AdaptiveHuffmanHeader* h);

#endif
