#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BITFILE_H
#define BITFILE_H

typedef struct{
    unsigned char* fileBuffer;
    unsigned int maxBufferSize;
    FILE* filePointer;
    int currentSize;
    int currentIndex;
    unsigned char bitBuffer;
    unsigned char currentByteSize;
    unsigned char lastByte_bitCount;
} bitReader;

typedef struct{
    unsigned char* fileBuffer;
    int maxSize;
    int currentIndex;
    unsigned char bitBuffer;
    unsigned char currentByteSize;
    long headerOffset;
    FILE* filePointer;
} bitWriter;

//Inicijalizuje niz bitova za zapisivanje
bitWriter* initBitWriter(FILE* bitFile,unsigned int bufferSize);

//Dodaje "numberOfBits" bitova unutar promjenjive "bits"
void addBits(bitWriter* savedBits,unsigned short bits,int numberOfBits);

//Upisuje ostatak bitova u fajl
void closeBitWriter(bitWriter* savedBits);

// Oslobadja dinamicki alociranu memoriju upisivaca bitova
void freeBitWriter(bitWriter* savedBits);

//Inicijalizuje niz bitova za citanje
bitReader* initBitReader(FILE* bitFile,unsigned int bufferSize);

//Ako nije kraj niza vraca sledeci bit (0/1), vraca 255 ako je dostignut kraj upisanog niza
int getBit(bitReader* savedBits);

unsigned int getBits(bitReader* savedBits,unsigned short count);

//Oslobadja dinamicki alociranu memoriju citaca bitova
void freeBitReader(bitReader* savedBits);

#endif

