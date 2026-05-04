#ifndef _WINDOWS_
	#include <windows.h>
#endif

static char** fileNames;
static int fileCount;

void recursiveDir(char* filepath){
	WIN32_FIND_DATA data;
	char buffer[1024];
	strcpy(buffer,filepath);
	strcat(buffer,"\\*");
    HANDLE hFind = FindFirstFileA(buffer, &data);

    if ( hFind != INVALID_HANDLE_VALUE ) {
        do {
        	if(strcmp(data.cFileName,".")==0 || strcmp(data.cFileName,"..")==0)continue;
        	
            
            strcpy(buffer,filepath);
			strcat(buffer,"\\"); 
			strcat(buffer,data.cFileName);
			            
            if(data.dwFileAttributes != INVALID_FILE_ATTRIBUTES && (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){ 
				recursiveDir(buffer); 
			}
			else{
				fileNames = (char**)realloc(fileNames,(fileCount+2)*sizeof(char*));
				int pathSize = strlen(buffer)-1;
				fileNames[fileCount] = (char*)malloc(pathSize * sizeof(char));
				fileNames[fileCount + 1] = NULL;
				strcpy(fileNames[fileCount],buffer+2);
				fileCount++;
			}
            
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }
	
}

void dirSingleFile(char* filepath){
	fileNames = (char**)realloc(fileNames,(fileCount+2)*sizeof(char*));
	int pathSize = strlen(filepath);
	fileNames[fileCount] = (char*)malloc(pathSize * sizeof(char));
	fileNames[fileCount + 1] = NULL;
	strcpy(fileNames[fileCount],filepath);
	fileCount++;
}

void handlePattern(char* pattern) {
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFileA(pattern, &data);

    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0)
            continue;

        
        dirSingleFile(data.cFileName);

    } while (FindNextFileA(hFind, &data));

    FindClose(hFind);
}

char** compressingFileNames(int argc,char** argv){
	fileNames = (char**)malloc(sizeof(char*));
	fileNames[0] = NULL;
	fileCount = 0;
	
	if(argc == 1){
		recursiveDir(".");
	}
	for(int i=1;i<argc;i++){
		if(strchr(argv[i], '*') || strchr(argv[i], '?')){
			handlePattern(argv[i]);
			continue;
		}
		if(argv[i][1]==':' || (argv[i][0]=='\\' && argv[i][1]=='\\') || (argv[i][0]=='.' && argv[i][1]=='\\')){//Apsolutna lokacija
			DWORD attribute = GetFileAttributes(argv[i]);
			if(attribute==INVALID_FILE_ATTRIBUTES)continue;
			if(attribute & FILE_ATTRIBUTE_DIRECTORY){ 
				recursiveDir(argv[i]); 
			} 
			else{ 
				dirSingleFile(argv[i]);
			}
			break;
		}
		char filename[1024] = {'.','\\',0};
		strcat(filename,argv[i]);
		DWORD attribute = GetFileAttributes(filename);
		if(attribute==INVALID_FILE_ATTRIBUTES)continue; 
		if(attribute & FILE_ATTRIBUTE_DIRECTORY){ 
			recursiveDir(filename); 
		} 
		else{
			dirSingleFile(filename+2);
		}
		
	}
	return fileNames;
}

void freeFileNames(){
	int i = 0;
    while(fileNames[i] != NULL){
    	free(fileNames[i]);
    	i++;
	}
	free(fileNames);
}