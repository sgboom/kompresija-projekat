typedef struct{
    unsigned char* fileBuffer;
    int maxSize;
    int currentIndex;
    unsigned char bitBuffer;
    unsigned char currentByteSize;
    FILE* filePointer;
} bitWriter;

bitWriter* initBitWriter(char* bitFileName,unsigned int bufferSize){
    bitWriter* result = (bitWriter*)malloc(sizeof(bitWriter));
    result->fileBuffer = malloc(bufferSize * sizeof(unsigned char));
    result->maxSize = bufferSize;
    result->currentIndex = 0;
    result->bitBuffer = '\0';
    result->currentByteSize = 0;
    result->filePointer = fopen(bitFileName,"wb");
    fputc(0,result->filePointer);
    return result;
}

void addBits(bitWriter* savedBits,unsigned short bits,int numberOfBits){
    if (numberOfBits <= 0 || numberOfBits > 16) return;
    bits &= (1 << numberOfBits) - 1; // dodatna provjera da osigura da gledamo samo nizih "numberOfBits"

    if(savedBits->currentIndex == savedBits->maxSize){
    	fwrite(savedBits->fileBuffer,1,savedBits->maxSize,savedBits->filePointer);
    	savedBits->currentIndex = 0;
	}
	
	if(savedBits->currentByteSize + numberOfBits > 8){
		int brojDostupnihBitova = 8 - savedBits->currentByteSize;
		savedBits->fileBuffer[savedBits->currentIndex++] = (savedBits->bitBuffer << brojDostupnihBitova) | (bits >> (numberOfBits - brojDostupnihBitova));
		savedBits->bitBuffer = '\0';
		numberOfBits -= brojDostupnihBitova;
		addBits(savedBits,bits,numberOfBits);
		return;
	}
	
	if(savedBits->currentByteSize + numberOfBits <= 8){
		savedBits->bitBuffer = (savedBits->bitBuffer << numberOfBits) | bits;
		savedBits->currentByteSize += numberOfBits;
		numberOfBits = 0;
	}
	
	if(savedBits->currentByteSize == 8){
		savedBits->fileBuffer[savedBits->currentIndex++] = savedBits->bitBuffer;
		savedBits->bitBuffer = '\0';
		savedBits->currentByteSize = 0;
		return;
	}
}

void closeBitWriter(bitWriter* savedBits){
    fwrite(savedBits->fileBuffer,1,savedBits->currentIndex,savedBits->filePointer);
    savedBits->bitBuffer <<= (8 - savedBits->currentByteSize);
    fputc(savedBits->bitBuffer,savedBits->filePointer);
    fseek(savedBits->filePointer,0,SEEK_SET);
    fputc(savedBits->currentByteSize,savedBits->filePointer);
}

void freeBitWriter(bitWriter* savedBits){
	fclose(savedBits->filePointer);
	free(savedBits->fileBuffer);
	free(savedBits);
}

