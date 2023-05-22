#ifndef HFCOMPRESSOR_FILESTRUCTURE_H
#define HFCOMPRESSOR_FILESTRUCTURE_H

#include <ctime>

class FileStructure;

class FileInside;

class HuffmanByte;

class HuffmanLeaf;

struct FileStructure {
    // header data
    const int MAGIC_NUMBER = 0x72; // Huffman -> Hf -> 72
    unsigned int fileCount=0;
    FileInside *fileInside{};
    // tree data
    HuffmanByte *huffmanByte{};
    unsigned long leafCount{};
    HuffmanLeaf *huffmanLeaf{};
    // data
    unsigned long long dataLength=0;
    // char* dataPart{};
};

struct FileInside {
    unsigned int pathSize{};
    char *name{};
    unsigned long long startByte{};
    unsigned long long endByte{};
    int offset = 0;

};

struct HuffmanByte {
    unsigned long partCount = 0;
    int offset = 0;
    unsigned char *treePart{};
};

struct HuffmanLeaf {
    unsigned int size{};
    char *dataPart{};
};

#endif //HFCOMPRESSOR_FILESTRUCTURE_H
