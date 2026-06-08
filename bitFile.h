#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BITFILE_H
#define BITFILE_H

#include "bitWriter.c"
#include "bitReader.c"

//Inicijalizuje niz bitova za zapisivanje
bitWriter* initBitWriter(char* bitFileName,unsigned int bufferSize);

//Dodaje "numberOfBits" bitova unutar promjenjive "bits"
void addBits(bitWriter* savedBits,unsigned short bits,int numberOfBits);

//Upisuje ostatak bitova u fajl
void closeBitWriter(bitWriter* savedBits);

// Oslobadja dinamicki alociranu memoriju upisivaca bitova
void freeBitWriter(bitWriter* savedBits);

//Inicijalizuje niz bitova za citanje
bitReader* initBitReader(char* bitFileName,unsigned int bufferSize);

//Ako nije kraj niza vraca sledeci bit (0/1), vraca 255 ako je dostignut kraj upisanog niza
int getBit(bitReader* savedBits);

//Oslobadja dinamicki alociranu memoriju citaca bitova
void freeBitReader(bitReader* savedBits);

#endif

