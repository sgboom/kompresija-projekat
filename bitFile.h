#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bitWriter.c"
#include "bitReader.c"

//Inicijalizuje niz bitova za zapisivanje
bitWriter* initBitWriter(int maxSize);

//Dodaje "numberOfBits" bitova unutar promjenjive "bits"
void addBits(bitWriter* savedBits,unsigned short bits,int numberOfBits);

//Inicijalizuje niz bitova za citanje
bitReader* initBitReader(char* bitFileName,unsigned int bufferSize);

//Ako nije kraj niza vraca sledeci bit (0/1), vraca 255 ako je dostignut kraj upisanog niza
int getBit(bitReader* savedBits);
