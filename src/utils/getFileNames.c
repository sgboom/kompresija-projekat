#include "../../include/parseFiles.h"
#include "../../include/huffman.h"
#include "../../include/lzw.h"

static wchar_t** fileNames; // TODO: Pretvori u ulancanu listu
static int fileCount;
unsigned int totalFilenameCharacters = 0;

void dirSingleFile(wchar_t* filepath){
	fileCount++;
	fileNames = (wchar_t**)realloc(fileNames,fileCount*sizeof(wchar_t*));
	
	int pathSize = wcslen(filepath)+1;
	totalFilenameCharacters += WideCharToMultiByte(CP_UTF8,0,filepath,-1,NULL,0,NULL,NULL);
	fileNames[fileCount-1] = (wchar_t*)malloc(pathSize * sizeof(wchar_t));
	wcscpy(fileNames[fileCount-1],filepath);
	//wprintf(L"%ls\n",fileNames[fileCount-1]);
}

//Mozda dodati maksimalnu rekurziju da ne bi pao sistem?
void recursiveDir(wchar_t* filepath){
	WIN32_FIND_DATAW data;
	wchar_t buffer[1024];
	wcscpy(buffer,filepath);
	int filepathSize = wcslen(filepath);
	if(buffer[filepathSize-1]!=L'\\'){
		buffer[filepathSize] = L'\\';
		buffer[filepathSize+1] = L'\0';
	}
	wcscat(buffer,L"*");
	//wprintf(L"%ls\n",buffer);
	
    HANDLE hFind = FindFirstFileW(buffer, &data);
	
    if ( hFind != INVALID_HANDLE_VALUE ){
        do {
        	if(wcscmp(data.cFileName,L".")==0 || wcscmp(data.cFileName,L"..")==0)continue;
        	
            wcscpy(buffer,filepath);
            
			int len = wcslen(buffer);
			if (buffer[len - 1] != L'\\'){
    			wcscat(buffer, L"\\");
			}

			wcscat(buffer,data.cFileName);
			            
            if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){ 
            	wcscat(buffer,L"\\");
            	dirSingleFile(buffer);
				recursiveDir(buffer); 
			}
			else{
				dirSingleFile(buffer);
			}
            
        } while (FindNextFileW(hFind, &data));
        FindClose(hFind);
    }
	
}



void handlePattern(wchar_t* filepath) {
    WIN32_FIND_DATAW data;
    HANDLE hFind = FindFirstFileW(filepath, &data);

    if (hFind == INVALID_HANDLE_VALUE) return;

	wchar_t directory[1024] = {0};
	wcscpy(directory,filepath);
	wchar_t* lastSlash = wcsrchr(directory,L'\\');
	if(lastSlash) *(lastSlash + 1) = L'\0';
	else directory[0] = L'\0';
	
    do {
        if (wcscmp(data.cFileName, L".") == 0 || wcscmp(data.cFileName, L"..") == 0)
            continue;
            
        wchar_t fullPath[1024];    

        if(directory[0]!=L'\0'){
        	wcscpy(fullPath,directory);
        	wcscat(fullPath,data.cFileName);
		}
		else wcscpy(fullPath,data.cFileName);
		
		dirSingleFile(fullPath);

    } while (FindNextFileW(hFind, &data));

    FindClose(hFind);
}

wchar_t** compressingFileNames(int argc,wchar_t** argv,int* fileNumber){
	fileNames = NULL;
	fileCount = 0;
	
	for(int i=1;i<argc;i++){
		if(argv[i][0]==L'\0' || argv[i][0]==L'-')continue;
		for(wchar_t* p = &argv[i][0];*p != L'\0';p++)if(*p == L'/')*p = L'\\';
		if(wcschr(argv[i], L'*') || wcschr(argv[i], L'?')){
			handlePattern(argv[i]);
			continue;
		}
		/*if(argv[i][1]==L':' || (argv[i][0]==L'\\' && argv[i][1]==L'\\') || (argv[i][0]==L'.' && argv[i][1]==L'\\')){//Apsolutna lokacija (bez njih pliz)
			DWORD attribute = GetFileAttributesW(argv[i]);
			if(attribute==INVALID_FILE_ATTRIBUTES)continue;
			if(attribute & FILE_ATTRIBUTE_DIRECTORY){ 
				recursiveDir(argv[i]); 
			} 
			else{ 
				dirSingleFile(argv[i]);
			}
			continue;
		}*/
		wchar_t filename[1024];
		wcscat(filename,argv[i]);
		DWORD attribute = GetFileAttributesW(filename);
		if(attribute==INVALID_FILE_ATTRIBUTES)continue; 
		if(attribute & FILE_ATTRIBUTE_DIRECTORY){ 
			int len = wcslen(filename);
			if (filename[len - 1] != L'\\'){
    			wcscat(filename, L"\\");
			}
			dirSingleFile(filename);
			recursiveDir(filename); 
		} 
		else{
			dirSingleFile(filename);
		}
		
	}
	*fileNumber = fileCount;
	return fileNames;
}

void writeToFile(wchar_t *fileName,wchar_t **fileNamesArray){
	FILE* filePointer = _wfopen(fileName,L"wb+");
	fwrite(&totalFilenameCharacters, sizeof(totalFilenameCharacters), 1, filePointer);
	printf("%u",totalFilenameCharacters);
	//fclose(filePointer);
	char buffer[1024];
	for(int i=0;i<fileCount;i++){
		int size = WideCharToMultiByte(CP_UTF8,0,fileNamesArray[i],-1,buffer,1024,NULL,NULL);
		fwrite(buffer,1,size-1,filePointer);
		fputc(';',filePointer);
	}
	WIN32_FILE_ATTRIBUTE_DATA fad;
	LARGE_INTEGER fileSize;
	unsigned char fileBuffer[65536];
	for(int i=0;i<fileCount;i++){
		if(wcscmp(fileNamesArray[i],fileName)==0)continue;
		//wprintf(L"%ls\n",fileNamesArray[i]);
		if(fileNamesArray[i][wcslen(fileNamesArray[i])-1]==L'\\')continue;
		
		GetFileAttributesExW(fileNamesArray[i], GetFileExInfoStandard, &fad);
		fileSize.LowPart =  fad.nFileSizeLow;
		fileSize.HighPart = fad.nFileSizeHigh;
		
		LARGE_INTEGER compressedFileSize;
		const wchar_t *currFileName = fileNamesArray[i];
		char flags = 0x000;
		if(LZW_Encode(currFileName,L"LZWEnkodovano.txt") == 0){
			GetFileAttributesExW(L"LZWEnkodovano.txt",GetFileExInfoStandard,&fad);
			compressedFileSize.LowPart =  fad.nFileSizeLow;
			compressedFileSize.HighPart = fad.nFileSizeHigh;
			if(compressedFileSize.QuadPart < fileSize.QuadPart){
				flags |= 4;
				currFileName = L"LZWEnkodovano.txt";
			}
		}
		adaptiveCompressFile(currFileName,L"HaffmanEnkodovano.txt");
		flags |= 1;	// 0b100
		
		if (!GetFileAttributesExW(L"HaffmanEnkodovano.txt", GetFileExInfoStandard, &fad)) {
    		wprintf(L"GetFileAttributesExW failed: %ls\n", fileNamesArray[i]);
    		continue;
		}
		fileSize.LowPart =  fad.nFileSizeLow;
		fileSize.HighPart = fad.nFileSizeHigh;
		fileSize.QuadPart |= ((unsigned long long)flags) << 61;
		fwrite(&fileSize.QuadPart,sizeof(fileSize.QuadPart),1,filePointer);
		fileSize.QuadPart &= ~(7ULL << 61);
		wprintf(L"%ls:%llu\t|%x|\n",fileNamesArray[i],fileSize.QuadPart,flags);
		FILE* secondFile = _wfopen(L"HaffmanEnkodovano.txt", L"rb");
		if(secondFile == NULL){
			printf("Greska??\n");
			continue;
		}
		unsigned int bytesRead;
		while ((bytesRead = fread(fileBuffer, 1, sizeof(fileBuffer), secondFile)) > 0) {
			printf(".");
    		fwrite(fileBuffer, 1, bytesRead, filePointer);
		}
    	
    	fclose(secondFile);
	}
	DeleteFile("HaffmanEnkodovano.txt");
	DeleteFile("LZWEnkodovano.txt");
	fclose(filePointer);
}

void freeFileNames(){
    for(int i=0;i<fileCount;i++){
    	free(fileNames[i]);
    	i++;
	}
	free(fileNames);
}