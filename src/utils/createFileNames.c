#include "../../include/parseFiles.h"

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
		FILE* secondFile = _wfopen(buffer,L"wb+");
		static unsigned char fileBuffer[65536 + 1];
		unsigned long long fileSize;
		fread(&fileSize,sizeof(unsigned long long),1,fptr);
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
	}
	
	fclose(fptr);
	free(fileNamesWideBuffer);
}