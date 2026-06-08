typedef struct{
    unsigned char* fileBuffer;
    unsigned int maxBufferSize;
    FILE* filePointer;
    int currentSize;
    int currentIndex;
    unsigned char bitBuffer;
    unsigned char currentByteSize;
    unsigned char lastByte_bitCount;
} bitReader;

bitReader* initBitReader(char* bitFileName,unsigned int bufferSize){
    bitReader* result = malloc(sizeof(bitReader));

    result->fileBuffer = calloc(bufferSize,sizeof(unsigned char));
    result->maxBufferSize = bufferSize;
    
    result->filePointer = fopen(bitFileName,"rb");

    result->currentSize = fread(result->fileBuffer,sizeof(char),bufferSize,result->filePointer);    //Ucitava prvih "bufferSize" bajtova u fileBuffer
    if(result->currentSize == 0){
		fclose(result->filePointer);
		return NULL;  //fajl je prazan
	}
    result->lastByte_bitCount = result->fileBuffer[0];
    result->currentIndex = 1;
    result->currentByteSize = 0;
    
    
    
    
    
    return result;
}

int getBit(bitReader* savedBits){
	if(savedBits->currentSize == 0)return -1;
    if(savedBits->currentByteSize == 0){
    	if(savedBits->currentIndex == savedBits->currentSize){
    		savedBits->currentSize = fread(savedBits->fileBuffer,1,savedBits->maxBufferSize,savedBits->filePointer);
			savedBits->currentIndex = 0;
			
			if(savedBits->currentSize == 0)return -1;
			
		}
		savedBits->bitBuffer = savedBits->fileBuffer[savedBits->currentIndex];
		
		if(savedBits->currentIndex == savedBits->currentSize - 1 && (feof(savedBits->filePointer) || (savedBits->currentSize < savedBits->maxBufferSize))){
			if(savedBits->lastByte_bitCount == 0)return -1;
			savedBits->currentByteSize = savedBits->lastByte_bitCount;
			savedBits->bitBuffer <<= (8-savedBits->lastByte_bitCount);
		}
		else savedBits->currentByteSize = 8;
		
		savedBits->currentIndex++;
	}
	savedBits->currentByteSize--;
	return (savedBits->bitBuffer >> savedBits->currentByteSize) & 1;
}

void freeBitReader(bitReader* savedBits){
	fclose(savedBits->filePointer);
	free(savedBits->fileBuffer);
	free(savedBits);
}