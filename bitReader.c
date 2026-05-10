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

    result->filePointer = fopen(bitFileName,"r");

    result->currentSize = fread(result->fileBuffer,sizeof(char),bufferSize,result->filePointer);    //Ucitava prvih "bufferSize" bajtova u fileBuffer
    if(result->currentSize == 0)return NULL;  //fajl je prazan

    result->lastByte_bitCount = result->fileBuffer[0];
    result->bitBuffer = result->fileBuffer[1];
    result->currentIndex = 1;
    result->currentByteSize = 8;
    return result;
}

int getBit(bitReader* savedBits){
    if(savedBits->currentByteSize > 0){
        int rez = (savedBits->bitBuffer >> 7) & 1;
        savedBits->bitBuffer <<= 1;
        savedBits->currentByteSize--;
        return rez;
    }
    if(savedBits->currentIndex < savedBits->currentSize - 1){
        savedBits->currentIndex++;
        if(savedBits->currentIndex == savedBits->currentSize - 1){
            if(savedBits->lastByte_bitCount == 0)return -1;
            savedBits->bitBuffer = savedBits->fileBuffer[savedBits->currentIndex] << (8 - savedBits->lastByte_bitCount);
            savedBits->currentByteSize = savedBits->lastByte_bitCount;
        }
        else{
            savedBits->bitBuffer = savedBits->fileBuffer[savedBits->currentIndex];
            savedBits->currentByteSize = 8;
        }
        int rez = (savedBits->bitBuffer >> 7) & 1;
        savedBits->bitBuffer <<= 1;
        savedBits->currentByteSize--;
        return rez;
    }
    savedBits->currentSize = fread(savedBits->fileBuffer,sizeof(unsigned char),savedBits->maxBufferSize,savedBits->filePointer);
    savedBits->currentIndex = 0;
    if(savedBits->currentSize <= 0)return -1;
    if(savedBits->currentSize == 1){
        if(savedBits->lastByte_bitCount == 0){
            savedBits->currentByteSize = 0;
            return -1;
        }
        savedBits->bitBuffer = savedBits->fileBuffer[0] << (8 - savedBits->lastByte_bitCount);
        savedBits->currentByteSize = savedBits->lastByte_bitCount;
    }
    else{
        
        savedBits->bitBuffer = savedBits->fileBuffer[0];
        savedBits->currentByteSize = 8;
    }

    int rez = (savedBits->bitBuffer >> 7) & 1;
    savedBits->bitBuffer <<= 1;
    savedBits->currentByteSize--;

    return rez;
}
