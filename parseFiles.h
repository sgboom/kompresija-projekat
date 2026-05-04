#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseFiles.c"

//Spaja sve nazive fajlova i stavlja ih u jedan string odvojen ';' , korisnik mora osloboditi alociranu memoriju unutar glavnog programa
char* loadFiles(char** fileNames,int fileCount);
