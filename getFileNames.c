static wchar_t** fileNames;
static int fileCount;

void recursiveDir(wchar_t* filepath){
	WIN32_FIND_DATAW data;
	wchar_t buffer[1024];
	wcscpy(buffer,filepath);
	wcscat(buffer,L"\\*");
    HANDLE hFind = FindFirstFileW(buffer, &data);

    if ( hFind != INVALID_HANDLE_VALUE ) {
        do {
        	if(wcscmp(data.cFileName,L".")==0 || wcscmp(data.cFileName,L"..")==0)continue;
        	
            
            wcscpy(buffer,filepath);
			wcscat(buffer,L"\\"); 
			wcscat(buffer,data.cFileName);
			            
            if(data.dwFileAttributes != INVALID_FILE_ATTRIBUTES && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){ 
				recursiveDir(buffer); 
			}
			else{
				fileNames = (wchar_t**)realloc(fileNames,(fileCount+2)*sizeof(wchar_t*));
				int pathSize = wcslen(buffer+2)+1;
				fileNames[fileCount] = (wchar_t*)malloc(pathSize * sizeof(wchar_t));
				fileNames[fileCount + 1] = NULL;
				wcscpy(fileNames[fileCount],buffer+2);
				fileCount++;
			}
            
        } while (FindNextFileW(hFind, &data));
        FindClose(hFind);
    }
	
}

void dirSingleFile(wchar_t* filepath){
	fileNames = (wchar_t**)realloc(fileNames,(fileCount+2)*sizeof(wchar_t*));
	int pathSize = wcslen(filepath)+1;
	fileNames[fileCount] = (wchar_t*)malloc(pathSize * sizeof(wchar_t));
	fileNames[fileCount + 1] = NULL;
	wcscpy(fileNames[fileCount],filepath);
	fileCount++;
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
	fileNames = (wchar_t**)malloc(sizeof(wchar_t*));
	fileNames[0] = NULL;
	fileCount = 0;
	
	if(argc == 1){
		recursiveDir(L".");
	}
	for(int i=1;i<argc;i++){
		for(wchar_t* p = &argv[i][0];*p != L'\0';p++)if(*p == L'/')*p = L'\\';
		if(wcschr(argv[i], L'*') || wcschr(argv[i], L'?')){
			handlePattern(argv[i]);
			continue;
		}
		if(argv[i][1]==L':' || (argv[i][0]==L'\\' && argv[i][1]==L'\\') || (argv[i][0]==L'.' && argv[i][1]==L'\\')){//Apsolutna lokacija
			DWORD attribute = GetFileAttributesW(argv[i]);
			if(attribute==INVALID_FILE_ATTRIBUTES)continue;
			if(attribute & FILE_ATTRIBUTE_DIRECTORY){ 
				recursiveDir(argv[i]); 
			} 
			else{ 
				dirSingleFile(argv[i]);
			}
			continue;
		}
		wchar_t filename[1024] = {L'.',L'\\',0};
		wcscat(filename,argv[i]);
		DWORD attribute = GetFileAttributesW(filename);
		if(attribute==INVALID_FILE_ATTRIBUTES)continue; 
		if(attribute & FILE_ATTRIBUTE_DIRECTORY){ 
			recursiveDir(filename); 
		} 
		else{
			dirSingleFile(filename+2);
		}
		
	}
	*fileNumber = fileCount;
	return fileNames;
}

void writeToFile(wchar_t *fileName,wchar_t **fileNamesArray,int fileCount){
	FILE* filePointer = _wfopen(fileName,L"w+,ccs=UTF-8");
	wchar_t krajJednogFajla = L';';
	for(int i=0;i<fileCount;i++){
		fputws(fileNamesArray[i],filePointer);
		if(i!=fileCount)fputwc(krajJednogFajla,filePointer);
	}
	krajJednogFajla = L'\0';
	fputwc(krajJednogFajla,filePointer);
	fclose(filePointer);
}

void freeFileNames(){
	int i = 0;
    while(fileNames[i] != NULL){
    	free(fileNames[i]);
    	i++;
	}
	free(fileNames);
}