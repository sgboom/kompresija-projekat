#include <windows.h>
#include <string.h>

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
				int pathSize = strlen(buffer+2)+1;
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
	int pathSize = strlen(filepath)+1;
	fileNames[fileCount] = (char*)malloc(pathSize * sizeof(char));
	fileNames[fileCount + 1] = NULL;
	strcpy(fileNames[fileCount],filepath);
	fileCount++;
}

void handlePattern(char* filepath) {
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFileA(filepath, &data);

    if (hFind == INVALID_HANDLE_VALUE) return;

	char directory[1024] = {0};
	strcpy(directory,filepath);
	char* lastSlash = strrchr(directory,'\\');
	if(lastSlash) *(lastSlash + 1) = '\0';
	else directory[0] = '\0';
	
	
    do {
        if (strcmp(data.cFileName, ".") == 0 || strcmp(data.cFileName, "..") == 0)
            continue;
            
        char fullPath[1024];    

        if(directory[0]!='\0'){
        	strcpy(fullPath,directory);
        	strcat(fullPath,data.cFileName);
		}
		else strcpy(fullPath,data.cFileName);
		
		dirSingleFile(fullPath);

    } while (FindNextFileA(hFind, &data));

    FindClose(hFind);
}

char** compressingFileNames(int argc,char** argv,int* fileNumber){
	fileNames = (char**)malloc(sizeof(char*));
	fileNames[0] = NULL;
	fileCount = 0;
	
	if(argc == 1){
		recursiveDir(".");
	}
	for(int i=1;i<argc;i++){
		for(char* p = &argv[i][0];*p != '\0';p++)if(*p == '/')*p = '\\';
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
			continue;
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
	*fileNumber = fileCount;
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