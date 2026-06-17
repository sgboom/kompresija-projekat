#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "../../include/lzw.h"
#include "../../include/bitFile.h"

void ADD_CHARS(decompressElement table[]) {
    for(int i=0;i<256;i++) {
    	table[i].string=calloc(2,sizeof(char));
        table[i].string[0]=(char)i;
        table[i].length = 1;
    }
    decompressElement prazan = {NULL,0};
    for (int i=256;i<N;i++) {
        table[i] = prazan;
    }
}
void LZW_DECODE(const wchar_t *filename_read,const wchar_t *filename_write) {
	decompressElement table[N];
    ADD_CHARS(table);
    int code=256;
    int code_length=8,new_code;
    char character[2] = {0};
    FILE *f=_wfopen(filename_read,L"rb");
    bitReader* reader = initBitReader(f,1024);
    FILE *out = _wfopen(filename_write,L"wb");
    int old_code = getBits(reader,code_length);
    fwrite(table[old_code].string,1,table[old_code].length,out);
    code_length++;
    while((new_code = getBits(reader, code_length)) != -1) {
        if (table[new_code].string==NULL) {
            fwrite(table[old_code].string,1,table[old_code].length,out);
            fputc(character[0],out);
            character[0]=table[old_code].string[0];
        }
        else {
            fwrite(table[new_code].string,1,table[new_code].length,out);
            character[0]=table[new_code].string[0];
        }        
        if (code!=N) {
            table[code].string = malloc(table[old_code].length + 2);
            strcpy(table[code].string,table[old_code].string);
            strcat(table[code].string,character);
            table[code].length = table[old_code].length + 1;
            code++;
        }
        if ((1<<code_length)==code) code_length++;
        old_code = new_code;
    }
    fclose(f);
    fclose(out);
    FREE_TABLE(table,code);
    freeBitReader(reader);
}
void FREE_TABLE(decompressElement table[],int codes) {
    for (int i=0;i<codes;i++) {
        free(table[i].string);
    }
}