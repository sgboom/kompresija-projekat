#include "../../include/bitFile.h"

bitWriter* initBitWriter(FILE* bitFile,unsigned int bufferSize){
    bitWriter* result = (bitWriter*)malloc(sizeof(bitWriter));
    result->fileBuffer = malloc(bufferSize * sizeof(unsigned char));
    result->maxSize = bufferSize;
    result->currentIndex = 0;
    result->bitBuffer = '\0';
    result->currentByteSize = 0;
    result->filePointer = bitFile;
	result->headerOffset = ftell(result->filePointer);

	fputc(0, result->filePointer);
    return result;
}

void addBits(bitWriter* savedBits,unsigned short bits,int numberOfBits){
	if (numberOfBits <= 0 || numberOfBits > 16) return;
	bits &= (1 << numberOfBits) - 1; // dodatna provjera da osigura da gledamo samo nizih "numberOfBits"s

	if(savedBits->currentIndex >= savedBits->maxSize - 2){
		fwrite(savedBits->fileBuffer,1,savedBits->currentIndex,savedBits->filePointer);
		savedBits->currentIndex = 0;
	}

	int brojDostupnihBitova = 8 - savedBits->currentByteSize;

	if(brojDostupnihBitova >= numberOfBits){
		savedBits->bitBuffer = (savedBits->bitBuffer << numberOfBits) | bits;
		savedBits->currentByteSize += numberOfBits;

		if(savedBits->currentByteSize == 8){
			savedBits->fileBuffer[savedBits->currentIndex++] = savedBits->bitBuffer;
			savedBits->bitBuffer = '\0';
			savedBits->currentByteSize = 0;
		}
		return;
	}

	savedBits->bitBuffer = (savedBits->bitBuffer << brojDostupnihBitova) | (bits >> (numberOfBits - brojDostupnihBitova));
	savedBits->fileBuffer[savedBits->currentIndex++] = savedBits->bitBuffer;
	savedBits->currentByteSize = 0;
	savedBits->bitBuffer = '\0';
	numberOfBits -= brojDostupnihBitova;
	bits &= (1 << numberOfBits) - 1;

	if(numberOfBits >= 8){
		brojDostupnihBitova = numberOfBits - 8;
		savedBits->fileBuffer[savedBits->currentIndex++] = bits >> brojDostupnihBitova;
		bits &= (1 << brojDostupnihBitova) - 1;
		savedBits->bitBuffer = bits;
		savedBits->currentByteSize = brojDostupnihBitova;
	}
	else{
		savedBits->bitBuffer = bits;
		savedBits->currentByteSize = numberOfBits;
	}
}

void closeBitWriter(bitWriter* savedBits){
	fwrite(savedBits->fileBuffer, 1, savedBits->currentIndex, savedBits->filePointer);
	fputc(savedBits->bitBuffer, savedBits->filePointer);
	fseek(savedBits->filePointer, savedBits->headerOffset, SEEK_SET);
	fputc(savedBits->currentByteSize, savedBits->filePointer);
}


void freeBitWriter(bitWriter* savedBits){
	fclose(savedBits->filePointer);
	free(savedBits->fileBuffer);
	free(savedBits);
}

