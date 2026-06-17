#include "../../include/parseFiles.h"
#include "../../include/huffman.h"
#include "../../include/lzw.h"

void createFiles(wchar_t *fileName,wchar_t *folderName){
	CreateDirectoryW(folderName,NULL);
	unsigned int fileNameCount;
	FILE* fptr = _wfopen(fileName,L"rb");
	fread(&fileNameCount,sizeof(unsigned int),1,fptr);
	//printf("%u\n",fileNameCount); 
	
	char *fileNamesBuffer = malloc(fileNameCount);
	fread(fileNamesBuffer,1,fileNameCount,fptr); 
	
	unsigned int wcharBufferSize = MultiByteToWideChar(CP_UTF8,0,fileNamesBuffer,fileNameCount,NULL,0);
	//printf("%u",wcharBufferSize);
	

	wchar_t* fileNamesWideBuffer = malloc((wcharBufferSize)*sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8,0,fileNamesBuffer,fileNameCount,fileNamesWideBuffer,wcharBufferSize);
	fileNamesWideBuffer[wcharBufferSize-1] = L'\0';
	//printf("Error: %lu\n", GetLastError());
	//for(int i=0;i<wcharBufferSize;i++){
	//	wprintf(L"%lc",fileNamesWideBuffer[i]);
	//}
	//wprintf(L"%ls",fileNamesWideBuffer);
	
	free(fileNamesBuffer);	
	
	wchar_t buffer[1024] = {0};
	char fileType = 'f';
	int folderNameSize = wcslen(folderName);
	wcscpy(buffer,folderName);
	wcscat(buffer,L"\\");
	//wprintf(L"%ls",buffer);
	int index = folderNameSize + 1;
	printf("%d",wcharBufferSize);
	for(int i=0;i<wcharBufferSize;i++){
		fileType = 'f';
		if(fileNamesWideBuffer[i]!=L';'){
			buffer[index++] = fileNamesWideBuffer[i];
			continue;
		}
		if(buffer[index - 1]==L'\\')fileType = 'd';
		buffer[index] = L'\0';
		wprintf(L"%ls\n",buffer);
		index = folderNameSize + 1;
		if(fileType == 'd'){
			CreateDirectoryW(buffer,NULL);
			continue;
		}
		
		FILE* secondFile = _wfopen(L"Enkodovano.txt",L"wb+");
		static unsigned char fileBuffer[65536 + 1];
		unsigned long long fileSize;
		fread(&fileSize,sizeof(unsigned long long),1,fptr);
		char flags = fileSize >> 61;
		fileSize &= ~(7ULL << 61);
		//printf("Flags:%d",flags);
		while(fileSize > 65536){
			fread(fileBuffer,1,65536,fptr);
			fwrite(fileBuffer,1,65536,secondFile);
			fileSize -= 65536;
		}
		if(fileSize > 0){
			fread(fileBuffer,1,fileSize,fptr);
			fwrite(fileBuffer,1,fileSize,secondFile);
		}
		fclose(secondFile);
		
		if(flags == 5){	//LZW + Dinamicki Huffman
			adaptiveDecompressFile(L"Enkodovano.txt",L"LZW.txt");
			LZW_DECODE(L"LZW.txt",buffer);
		}
		else if(flags == 1){
			adaptiveDecompressFile(L"Enkodovano.txt",buffer);
		}
		else if (flags == 0){
			MoveFileExW(L"Enkodovano.txt",buffer,MOVEFILE_COPY_ALLOWED);
		}
		
		//LZW_DECODE(L"HaffmanDekodovano.txt",buffer);
	}
	DeleteFile("Enkodovano.txt");
	DeleteFile("LZW.txt");
	fclose(fptr);
	free(fileNamesWideBuffer);
}