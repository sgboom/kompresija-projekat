char* loadFiles(char** fileNames,int fileCount){
	int nameSize = 1;
	int* fileNameSizes = (int*)malloc(fileCount * sizeof(int));
	for(int i=0;i<fileCount;i++){
		fileNameSizes[i] = strlen(fileNames[i]);
		nameSize += fileNameSizes[i] + 1;
	}
	char* files = calloc(nameSize,sizeof(char));
	char* endOfLastName = files;	
	for(int i=0;i<fileCount;i++){
		memcpy(endOfLastName,fileNames[i],fileNameSizes[i]);
		endOfLastName += fileNameSizes[i];
		*endOfLastName = ';';
		endOfLastName++;
	}
	*endOfLastName = '\0';
	free(fileNameSizes);
	return files;
}