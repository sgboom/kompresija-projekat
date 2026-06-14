cmake_minimum_required(VERSION 4.1)
project(kompresija-projekat C)

set(CMAKE_C_STANDARD 11)

add_executable(kompresija-projekat src/main.c
        src/bitFile/bitReader.c
        src/bitFile/bitWriter.c
        src/huffman/adaptiveHuffman.c
        src/data-structures/minHeap.c
        src/huffman/staticHuffman.c
        src/utils/getFileNames.c
        src/utils/createFileNames.c
        src/data-structures/hashTable.c
        include/lzw.h
        src/lzw/DecompressLZW.c
        src/lzw/CompressLZW.c)
