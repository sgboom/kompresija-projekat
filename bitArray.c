extern unsigned int INITIAL_MAX_SIZE;

unsigned char* bitArray = NULL;
int maxSize = 0;
int currentIndex = 0;

unsigned char bitBuffer = '\0';
unsigned char currentByteSize = 0;

void initBitArray(){
    bitArray = malloc(INITIAL_MAX_SIZE * sizeof(unsigned char));
    maxSize = INITIAL_MAX_SIZE;
    currentIndex = 0;
    bitBuffer = '\0';
    currentByteSize = 0;
    return;
}

void addBits(unsigned short bits,int numberOfBits){
    if (numberOfBits <= 0 || numberOfBits > 16) return;
    bits &= (1 << numberOfBits) - 1; // dodatna provjera da osigura da gledamo samo nizih "numberOfBits"

    if(currentIndex == maxSize - 1){
        maxSize *= 2;        
        bitArray = realloc(bitArray,maxSize * sizeof(unsigned char));
    }
    if(numberOfBits == 8 && currentByteSize == 0){
        bitArray[currentIndex] = bits;
        currentIndex++;
        return;

    }
    if(currentByteSize + numberOfBits <= 8){
        bitBuffer  = bitBuffer << numberOfBits;
        bitBuffer |= bits;
        currentByteSize += numberOfBits;
        if(currentByteSize== 8){
            currentByteSize = 0;
            bitArray[currentIndex] = bitBuffer;
            bitBuffer = '\0';
            currentIndex++;
        }
        return;
    }
    
    int brojDostupnihBitova = 8-currentByteSize;
    bitBuffer  = bitBuffer << brojDostupnihBitova;
    bitBuffer |= bits >> (numberOfBits - brojDostupnihBitova);
    numberOfBits -= brojDostupnihBitova;
    bits &= (1 << numberOfBits) - 1;
    
    bitArray[currentIndex] = bitBuffer;
    bitBuffer = '\0';
    currentIndex++;
    
    if(currentIndex == maxSize - 1){
        maxSize *= 2;        
        bitArray = realloc(bitArray,maxSize * sizeof(unsigned char));
    }

    if(numberOfBits < 8){
        bitBuffer = bits;
        currentByteSize = numberOfBits;
        return;
    }
    if(numberOfBits == 8){
        bitArray[currentIndex] = bits;
        currentIndex++;
        return;
    }
    
    bitArray[currentIndex] = bits>>(numberOfBits-8);
    currentIndex++;
    bitBuffer = bits & ((1 << (numberOfBits - 8)) - 1);
    currentByteSize = numberOfBits - 8;
    return;
}

unsigned char* getBitArray(int* size){
    if(currentByteSize != 0){
        bitArray[currentIndex] = bitBuffer << (8 - currentByteSize);
        currentIndex++;
    }
    *size = currentIndex;
    return bitArray;
}
