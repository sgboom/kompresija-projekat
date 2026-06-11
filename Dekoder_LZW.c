#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int N=4096;
unsigned long r_buffer = 0;
int r_bit_count = 0;
void FREE_TABLE(char* table[],int codes);
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
}
void ADD_CHARS(char* table[]) {
     if (table==NULL) {
          exit(-1);
     }
     for(int i=0;i<256;i++) {
          table[i]=calloc(2,sizeof(char));
          table[i][0]=(char)i;
     }
}
void ADD_STRING(char* table[],char str[],int code) {
     if (table==NULL) {
          exit(-1);
     }
     table[code]=calloc(strlen(str)+1,sizeof(char));
     strcpy(table[code],str);
}
void LZW_DECODE(char*** table,char filename_read[],char filename_write[]) {
     int code=256;
     int code_length=8,new_code;
     if (table==NULL) {
          exit(-1);
     }
     char character[2] = {0};
     FILE *f=fopen(filename_read,"rb");
     FILE *out=fopen(filename_write,"w");
     int old_code=ReadBits(f,code_length);
     fprintf(out, "%s", (*table)[old_code]);
     code_length++;
     while((new_code = ReadBits(f, code_length)) != -1) {
          char*temp=NULL;
          if ((*table)[new_code]==NULL) {
               temp=malloc((strlen((*table)[old_code])+2)*sizeof(char));
               if (temp==NULL) {
                    free(temp);
                    fclose(f);
                    fclose(out);
                    FREE_TABLE(*table,code);
                    exit(-1);
               }
               strcpy(temp,(*table)[old_code]);
               strcat(temp,character);
          }
          else {
               temp=malloc((strlen((*table)[new_code])+1)*sizeof(char));
               if (temp==NULL) {
                    free(temp);
                    fclose(f);
                    fclose(out);
                    FREE_TABLE((*table),code);
                    exit(-1);
               }
               strcpy(temp,(*table)[new_code]);
               temp[strlen(temp)]='\0';
          }
          fprintf(out, "%s", temp);
          character[0]=temp[0];
          char*novi=malloc((strlen((*table)[old_code]) + 2) * sizeof(char));
          strcpy(novi,(*table)[old_code]);
          strcat(novi,character);
          if (code >= N) {
               int old_N=N;
               N=N*2;
               char** temp_table=realloc(*table, N * sizeof(char*));
               if (temp_table == NULL) {
                    exit(-1);
               }
               *table=temp_table;
               for(int i=old_N;i<N;i++) {
                    (*table)[i]=NULL;
               }
          }
          ADD_STRING(*table,novi,code++);
          free(novi);
          free(temp);
          if ((1<<code_length)==code) code_length++;
          old_code = new_code;
     }
     fclose(f);
     fclose(out);
     FREE_TABLE(*table,code);
}
void FREE_TABLE(char* table[],int codes) {
     if (table==NULL) {
          exit(-1);
     }
     for (int i=0;i<N;i++) {
          free(table[i]);
     }
     free(table);
}
int main() {
     char** decoder=calloc(N,sizeof(char*));
     ADD_CHARS(decoder);
     LZW_DECODE(&decoder,"coded.txt","decoded.txt");
}