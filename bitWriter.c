typedef struct{
    unsigned char* array;
    int maxSize;
    int currentIndex;
    unsigned char bitBuffer;
    unsigned char currentByteSize;
} bitWriter;

bitWriter* initBitWriter(int initialMaxSize){
    bitWriter* result = (bitWriter*)malloc(sizeof(bitWriter));
    result->array = malloc(initialMaxSize * sizeof(unsigned char));
    result->maxSize = initialMaxSize;
    result->currentIndex = 0;
    result->bitBuffer = '\0';
    result->currentByteSize = 0;
    return result;
}

void addBits(bitWriter* savedBits,unsigned short bits,int numberOfBits){
    if (numberOfBits <= 0 || numberOfBits > 16) return;
    bits &= (1 << numberOfBits) - 1; // dodatna provjera da osigura da gledamo samo nizih "numberOfBits"

    if(savedBits->currentIndex == savedBits->maxSize - 1){
        savedBits->maxSize *= 2;        
        savedBits->array = realloc(savedBits->array,savedBits->maxSize * sizeof(unsigned char));
    }
    if(numberOfBits == 8 && savedBits->currentByteSize == 0){
        savedBits->array[savedBits->currentIndex] = bits;
        savedBits->currentIndex++;
        return;

    }
    if(savedBits->currentByteSize + numberOfBits <= 8){
        savedBits->bitBuffer  = savedBits->bitBuffer << numberOfBits;
        savedBits->bitBuffer |= bits;
        savedBits->currentByteSize += numberOfBits;
        if(savedBits->currentByteSize== 8){
            savedBits->currentByteSize = 0;
            savedBits->array[savedBits->currentIndex] = savedBits->bitBuffer;
            savedBits->bitBuffer = '\0';
            savedBits->currentIndex++;
        }
        return;
    }
    
    int brojDostupnihBitova = 8-savedBits->currentByteSize;
    savedBits->bitBuffer  = savedBits->bitBuffer << brojDostupnihBitova;
    savedBits->bitBuffer |= bits >> (numberOfBits - brojDostupnihBitova);
    numberOfBits -= brojDostupnihBitova;
    bits &= (1 << numberOfBits) - 1;
    
    savedBits->array[savedBits->currentIndex] = savedBits->bitBuffer;
    savedBits->bitBuffer = '\0';
    savedBits->currentIndex++;
    
    if(savedBits->currentIndex == savedBits->maxSize - 1){
        savedBits->maxSize *= 2;        
        savedBits->array = realloc(savedBits->array,savedBits->maxSize * sizeof(unsigned char));
    }

    if(numberOfBits < 8){
        savedBits->bitBuffer = bits;
        savedBits->currentByteSize = numberOfBits;
        return;
    }
    if(numberOfBits == 8){
        savedBits->array[savedBits->currentIndex] = bits;
        savedBits->currentIndex++;
        return;
    }
    
    savedBits->array[savedBits->currentIndex] = bits>>(numberOfBits-8);
    savedBits->currentIndex++;
    savedBits->bitBuffer = bits & ((1 << (numberOfBits - 8)) - 1);
    savedBits->currentByteSize = numberOfBits - 8;
    return;
}

void writeBitsToFile(bitWriter* savedBits,char* fileName){
    FILE* filePointer = fopen(fileName,"w");
    fwrite(&savedBits->currentByteSize,sizeof(unsigned char),1,filePointer);   // Prvi bajt ce biti broj bitova koji ostaju u trenutnom bufferu
    fwrite(savedBits->array,sizeof(unsigned char),savedBits->currentIndex,filePointer);//Upisuje ostale bitove
    if(savedBits->currentByteSize != 0)fwrite(&savedBits->bitBuffer,sizeof(unsigned char),1,filePointer);
    fclose(filePointer);
}

