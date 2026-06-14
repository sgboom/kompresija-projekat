#include <stdio.h>
#include <wchar.h>
#include "../include/parseFiles.h"
int main() {
	char method = 'n';	// 'c' za kompresovanje i 'd' za dekompresovanje
	wchar_t *outputFile;
	int argc;
	wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	for(int i=1;i<argc;i++){
		if(argv[i][0]==L'-'){
			if(argv[i][1]==L'h' || argv[i][1] == L'?'){
				printf("NinjaZIP - Alat za kompresiju podataka\n");
				printf("Upotreba: ./NinjaZip.exe [ARGUMENTI]\n\n");
				printf("Opcije:\n");
				printf("	-h,-?			Ispisuje ovaj meni\n");
				printf("	-a			Ispisuje osnovne informacije o alatu\n");
				printf("	-l			Ispisuje korake tokom obavljanja kompresovanja i dekompresovanja\n");
				printf("	-c			Dekompresuje zadatu datoteku\n");
				printf("	-d			Dekompresuje zadatu datoteku\n");
				printf("	-o			Naziv izlaznog fajla\n");
				printf("Primer:\n\t./NinjaZip.exe -c . -o izlaznifajl.nzip\n\t./NinjaZip.exe -d izlaznifajl.nzip -o odzipovanFolder");
				return 0;
			}
			if(argv[i][1]==L'a'){
				printf("NinjaZip je alat za kompresiju i dekompresiju podataka...\n");
			}
			if(argv[i][1]==L'l')printf("Mrzim logovanje!");
			if(argv[i][1]==L'd'){
				//printf("Dekompresovanje\n");
				method = 'd';
			}
			if(argv[i][1]==L'c'){
				//printf("Kompresovanje\n");
				//for(int j=1;j<argc;j++)wprintf(L"%d:%ls\n",j,argv[j]);
				method = 'c';
			}
			if(argv[i][1]==L'o'){
				//printf("Output File\n");
				if(i+1 >= argc){
					printf("Nije zadat output fajl!\n");
					return -1;
				}
				outputFile = malloc((wcslen(argv[i+1])+1)*sizeof(wchar_t));
				wcscpy(outputFile,argv[i+1]);
				argv[i+1][0] = L'\0';
				wprintf(L"out:%ls\n",outputFile);
			}
		}
	}
	if(method == 'n'){
		printf("Error: Nije izabrana kompresija/dekompresija");
		return 0;
	}
	if(method == 'c'){
		//DeleteFileW(outputFile);
		int fileNumber;
    	wchar_t** fileNames = compressingFileNames(argc,argv,&fileNumber);
		writeToFile(outputFile,fileNames);
		//printf("%d",fileNumber);
		freeFileNames();
	}
	if(method == 'd'){
		for(int i=1;i<argc;i++){
			if(argv[i][0]==L'\0' || argv[i][0]==L'-')continue;
			createFiles(argv[i],outputFile);
		}

	}
	free(outputFile);

	return 0;

}