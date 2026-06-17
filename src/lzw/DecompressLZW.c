#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "../../include/lzw.h"
#include "../../include/bitFile.h"

/*unsigned long r_buffer = 0;
int r_bit_count = 0;
int ReadBits(FILE* f,int length) {
     while (r_bit_count < length) {
          int byte=fgetc(f);
          if (byte==EOF) {
               if (r_bit_count==0) {
                    return -1;
               }
               byte=0;
          }
          r_buffer=(r_buffer<<8)|(unsigned char)byte;
          r_bit_count+=8;
     }
     int shrift=r_bit_count-length;
     int kod=(int)((r_buffer>>shrift)&((1<<length)-1));
     r_bit_count-=length;
     r_buffer&=(1<<shrift)-1;
     return kod;
}*/
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
     //int old_code=ReadBits(f,code_length);
     int old_code = getBits(reader,code_length);
     //printf("%d|%s:%d\n", old_code,table[old_code].string,table[old_code].length); //AAAAAAA
     fwrite(table[old_code].string,1,table[old_code].length,out);
     //character[0] = table[old_code].string[0];
     code_length++;
     while((new_code = getBits(reader, code_length)) != -1) {
     	//printf("%d",new_code);
          if (table[new_code].string==NULL) {
               /*temp=malloc((strlen(table[old_code])+2)*sizeof(char));
               if (temp==NULL) {
                    free(temp);
                    fclose(f);
                    fclose(out);
                    FREE_TABLE(table,code);
                    exit(-1);
               }*/
               fwrite(table[old_code].string,1,table[old_code].length,out);
               fputc(character[0],out);
               //printf("%d|%s%c:%d\n",old_code, table[old_code].string,character[0],table[old_code].length);
               character[0]=table[old_code].string[0];
               //strcpy(temp,table[old_code]);
               //strcat(temp,character);
          }
          else {
               /*temp=malloc((strlen(table[new_code])+1)*sizeof(char));
               if (temp==NULL) {
                    free(temp);
                    fclose(f);
                    fclose(out);
                    FREE_TABLE(table,code);
                    exit(-1);
               }*/
               fwrite(table[new_code].string,1,table[new_code].length,out);
               character[0]=table[new_code].string[0];
               //printf("%d|%s:%d\n",new_code, table[new_code].string,table[new_code].length);
               //strcpy(temp,table[new_code]);
               //temp[strlen(temp)]='\0';
          }
          //fprintf(out, "%s", temp);
          
          if (code!=N) {
          		
               	table[code].string = malloc(table[old_code].length + 2);
               	strcpy(table[code].string,table[old_code].string);
               	strcat(table[code].string,character);
               	table[code].length = table[old_code].length + 1;
               	code++;
          }
          //free(temp);
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

