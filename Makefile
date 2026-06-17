OBJECTS = bitReader.o bitWriter.o hashTable.o minHeap.o adaptiveHuffman.o staticHuffman.o CompressLZW.o DecompressLZW.o createFileNames.o getFileNames.o

CC = gcc
CFLAGS = -Wall -Wextra -l lstdlib

all: main.exe

main.exe: $(OBJECTS)
	gcc src/main.c $(OBJECTS) -o ninja-zip.exe

bitReader.o:
	$(CC) -c src/bitFile/bitReader.c -o $@
bitWriter.o: 
	$(CC) -c src/bitFile/bitWriter.c -o $@
hashTable.o: 
	$(CC) -c src/data-structures/hashTable.c -o $@ 
minHeap.o: 
	$(CC) -c src/data-structures/minHeap.c -o $@ 
adaptiveHuffman.o: 
	$(CC) -c src/huffman/adaptiveHuffman.c -o $@
staticHuffman.o: 
	$(CC) -c src/huffman/staticHuffman.c -o $@
CompressLZW.o: 
	$(CC) -c src/lzw/CompressLZW.c -o $@
DecompressLZW.o: 
	$(CC) -c src/lzw/DecompressLZW.c -o $@
getFileNames.o: 
	$(CC) -c src/utils/getFileNames.c -o $@
createFileNames.o: 
	$(CC) -c src/utils/createFileNames.c -o $@

clean:
	del *.o