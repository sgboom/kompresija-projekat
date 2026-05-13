#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

#include "getFileNames.c"
//#include "createFileNames.c" -- TODO

//Poziva se u slucaju foldera i ako se pronadje jos jedan folder rekurzivno poziva i na tom folderu
void recursiveDir(wchar_t* filepath);

//Dodaje jedan fajl, NE PROVJERAVA da li je fajl validan
void dirSingleFile(wchar_t* filepath);

//Poziva se ako ima '?' ili '*' zato sto mi je mrsko bilo da napravim unutar obicne funkcije
void handlePattern(wchar_t* filepath);

//Vraca NULL-terminirani niz naziva fajlova,i mijenja pokazivac fileNumber da vrati broj fajlova
wchar_t** compressingFileNames(int argc,wchar_t** argv,int* fileNumber);

//Oslobadja dinamicki alociran prostor za cuvanje naziva fajlova
void freeFileNames();

//Ucitava fajlove unutar "fileNamesArray" kojih ima "fileCount" u fajl cije je ime "fileName"
void writeToFile(wchar_t *fileName,wchar_t **fileNamesArray,int fileCount);