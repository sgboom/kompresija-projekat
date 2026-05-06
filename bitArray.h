#include <stdlib.h>
#include "bitArray.c"

//Koliko bajtova inicijalizuje na pocetku
unsigned int INITIAL_MAX_SIZE = 1024;

//Inicijalizuje niz bitova
void initBitArray();

//Dodaje "numberOfBits" bitova unutar promjenjive "bits"
void addBits(unsigned short bits,int numberOfBits);

//Vraca niz bajtova !!! Nakon vracanja niz fakticki postaje read-only zbog moranja dodavanja bitova koji ne cine citav bajt !!!
unsigned char* getBitArray(int* size);
