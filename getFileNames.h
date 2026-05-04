#include "getFileNames.c"

//Poziva se u slucaju foldera i ako se pronadje jos jedan folder rekurzivno poziva i na tom folderu
void recursiveDir(char* filepath);

//Dodaje jedan fajl, NE PROVJERAVA da li je fajl validan
void dirSingleFile(char* filepath);

//Poziva se ako ima '?' ili '*' zato sto mi je mrsko bilo da napravim unutar obicne funkcije
void handlePattern(char* filepath);

//Vraca NULL-terminirani niz naziva fajlova,i mijenja pokazivac fileNumber da vrati broj fajlova
char** compressingFileNames(int argc,char** argv,int* fileNumber);

//Oslobadja dinamicki alociran prostor za cuvanje naziva fajlova
void freeFileNames();