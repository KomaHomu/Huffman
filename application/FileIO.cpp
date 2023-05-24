#ifndef HFCOMPRESSOR_FILEIO_CPP
#define HFCOMPRESSOR_FILEIO_CPP

#include <cstdio>
#include <stdexcept>
#include <vector>
#include "FileStructure.h"
#include "HuffmanTree.h"
#include "FileIO.h"
#include <filesystem>
#include <cstring>

using namespace std;
namespace fs = filesystem;

void FileIO::writeHeader() const {
    FILE *file = fopen(path, "wb");
    fwrite(&fileStructure->MAGIC_NUMBER, sizeof(int), 1, file);
    // write files info
    fwrite(&fileStructure->fileCount, sizeof(unsigned int), 1, file);
    for (unsigned int i = 0; i < fileStructure->fileCount; ++i) {
        fwrite(&fileStructure->fileInside[i].pathSize, sizeof(unsigned int), 1, file);
        fwrite(fileStructure->fileInside[i].name, sizeof(char), fileStructure->fileInside[i].pathSize, file);
        fwrite(&fileStructure->fileInside[i].startByte, sizeof(unsigned long long), 1, file);
        fwrite(&fileStructure->fileInside[i].endByte, sizeof(unsigned long long), 1, file);
        fwrite(&fileStructure->fileInside[i].offset, sizeof(int), 1, file);
    }
    generateTreeData();
    // write tree data
    fwrite(&fileStructure->huffmanByte->partCount, sizeof(unsigned long), 1, file);
    fwrite(&fileStructure->huffmanByte->offset, sizeof(int), 1, file);
    fwrite(fileStructure->huffmanByte->treePart, sizeof(unsigned char), fileStructure->huffmanByte->partCount, file);
    fwrite(&fileStructure->leafCount, sizeof(unsigned int), 1, file);
    for (unsigned long i = 0; i < fileStructure->leafCount; ++i) {
        fwrite(&fileStructure->huffmanLeaf[i].size, sizeof(unsigned int), 1, file);
        fwrite(fileStructure->huffmanLeaf[i].dataPart, sizeof(char), fileStructure->huffmanLeaf[i].size, file);
    }
}

void FileIO::generateTreeData() const {
    // flat the tree
    vector<bool> code = vector<bool>();
    huffmanTreeRoot->flat(&code);
    fileStructure->huffmanByte->treePart = (unsigned char *) malloc(0);
    bool remaining = false;
    for (bool i: code) {
        remaining = true;
        data->set((7 - fileStructure->huffmanByte->offset), i);
        fileStructure->huffmanByte->offset = (fileStructure->huffmanByte->offset + 1) % 8;
        if (fileStructure->huffmanByte->offset == 0) {
            fileStructure->huffmanByte->partCount++;
            fileStructure->huffmanByte->treePart = (unsigned char *) realloc(fileStructure->huffmanByte->treePart,
                                                                             fileStructure->huffmanByte->partCount);
            fileStructure->huffmanByte->treePart[fileStructure->huffmanByte->partCount -
                                                 1] = static_cast<unsigned char>(data->to_ulong());
            data->reset();
            remaining = false;
        }
    }
    if (remaining) {
        fileStructure->huffmanByte->partCount++;
        fileStructure->huffmanByte->treePart = (unsigned char *) realloc(fileStructure->huffmanByte->treePart,
                                                                         fileStructure->huffmanByte->partCount);
        fileStructure->huffmanByte->treePart[fileStructure->huffmanByte->partCount -
                                             1] = static_cast<unsigned char>(data->to_ulong());
    }
    vector<HuffmanTree *> leafs = vector<HuffmanTree *>();
    huffmanTreeRoot->leaf(leafs);
    // generate leafs data
    fileStructure->huffmanLeaf = (HuffmanLeaf *) malloc(leafs.size());
    fileStructure->leafCount = leafs.size();
    for (int i = 0; i < leafs.size(); ++i) {
        fileStructure->huffmanLeaf[i].size = leafs[i]->size;
        fileStructure->huffmanLeaf[i].dataPart = leafs[i]->dataPart;
    }
}

void FileIO::addFile(char *path) const {
    fs::path filePath = fs::path(path);
    if (!fs::exists(filePath)) {
        throw runtime_error("File not found: " + string(path));
    }
    if (fs::is_directory(filePath)) {
        throw runtime_error("Path is a directory: " + string(path));
    }
    fileStructure->fileCount++;
    if (fileStructure->fileCount > 1) {
        if (data->to_ulong() != 0) {
            unsigned char flushChar = static_cast<unsigned char>(data->to_ulong());
            FILE *file = fopen(pathTempData, "wb");
            fwrite(&flushChar, sizeof(unsigned char), 1, file);
            fileStructure->dataLength++;
            fileStructure->fileInside[fileStructure->fileCount - 2].endByte++;
            data->reset();
        }
    }
    char *name = (char *) filePath.relative_path().c_str();
    char *current_pos;
    for (char *p = name; (current_pos = strchr(name, fs::path::preferred_separator)) != nullptr; *current_pos = ';');
    fileStructure->fileInside = (FileInside *) realloc(fileStructure->fileInside, fileStructure->fileCount);
    fileStructure->fileInside[fileStructure->fileCount - 1].name = name;
    fileStructure->fileInside[fileStructure->fileCount - 1].pathSize = sizeof(fileStructure->fileInside[
            fileStructure->fileCount - 1].name);
    fileStructure->fileInside[fileStructure->fileCount - 1].startByte = fileStructure->dataLength;
    fileStructure->fileInside[fileStructure->fileCount - 1].endByte = fileStructure->dataLength;
}

void FileIO::writeBitData(bool bit) const {
    FILE *file = fopen(pathTempData, "wb");
    data->set(7 - fileStructure->fileInside[fileStructure->fileCount - 1].offset, bit);
    fileStructure->fileInside[fileStructure->fileCount - 1].offset =
            (fileStructure->fileInside[fileStructure->fileCount - 1].offset + 1) % 8;
    if (fileStructure->fileInside[fileStructure->fileCount - 1].offset == 0) {
        unsigned char flushChar = static_cast<unsigned char>(data->to_ulong());
        fwrite(&flushChar, sizeof(unsigned char), 1, file);
        data->reset();
        fileStructure->dataLength++;
        fileStructure->fileInside[fileStructure->fileCount - 1].endByte++;
    }
}


void FileIO::flush() const {
    writeHeader();
    // copy pathTempData to path
    FILE *file = fopen(path, "wba");
    FILE *fileTemp = fopen(pathTempData, "rb");
    char buffer;
    while (fread(&buffer, sizeof(char), 1, fileTemp) == 1) {
        fwrite(&buffer, sizeof(char), 1, file);
    }
    fclose(file);
    fclose(fileTemp);
    remove(pathTempData);
}

void FileIO::readHeader() const {
    FILE *file = fopen(path, "wb");
    int magicNumber;
    fread(&magicNumber, sizeof(int), 1, file);
    if (magicNumber != fileStructure->MAGIC_NUMBER) {
        throw runtime_error("Error processing file: " + string(path));
    }
    // read files info
    fread(&fileStructure->fileCount, sizeof(unsigned int), 1, file);
    fileStructure->fileInside = new FileInside[fileStructure->fileCount];
    for (unsigned int i = 0; i < fileStructure->fileCount; ++i) {
        fread(&fileStructure->fileInside[i].pathSize, sizeof(unsigned int), 1, file);
        fread(fileStructure->fileInside[i].name, sizeof(char), fileStructure->fileInside[i].pathSize, file);
        fread(&fileStructure->fileInside[i].startByte, sizeof(unsigned long long), 1, file);
        fread(&fileStructure->fileInside[i].endByte, sizeof(unsigned long long), 1, file);
    }
    // read tree data
    fread(&fileStructure->huffmanByte->partCount, sizeof(unsigned int), 1, file);
    fileStructure->huffmanByte->treePart = (unsigned char *) malloc(fileStructure->huffmanByte->partCount);
    fread(&fileStructure->huffmanByte->offset, sizeof(int), 1, file);
    fread(fileStructure->huffmanByte->treePart, sizeof(unsigned char), fileStructure->huffmanByte->partCount, file);
    fread(&fileStructure->leafCount, sizeof(unsigned int), 1, file);
    fileStructure->huffmanLeaf = (HuffmanLeaf *) malloc(fileStructure->leafCount);
    for (unsigned int i = 0; i < fileStructure->leafCount; ++i) {
        fread(&fileStructure->huffmanLeaf[i].size, sizeof(unsigned int), 1, file);
        fread(&fileStructure->huffmanLeaf[i].dataPart, sizeof(unsigned char), 1, file);
    }
    fread(&fileStructure->dataLength, sizeof(unsigned long long), 1, file);
}

void FileIO::treeBuild() const {
    vector<HuffmanEntry> huffmanTables = vector<HuffmanEntry>();
    vector<bool> code = vector<bool>();
    for (unsigned long i = 0; i < fileStructure->leafCount; ++i) {
        HuffmanEntry tmp = HuffmanEntry();
        tmp.huffmanLeaf = &fileStructure->huffmanLeaf[i];
        huffmanTables.push_back(tmp);
    }
    bitset<8> tmp;
    for (unsigned long i = 0; i < fileStructure->huffmanByte->partCount; ++i) {
        tmp = bitset<8>(fileStructure->huffmanByte->treePart[i]);
        for (int j = 0; j < 8; ++j) {
            code.push_back(tmp[7 - j]);
        }
    }
    for (int i = 0; i < fileStructure->huffmanByte->offset; ++i) {
        code.pop_back();
    }
    *huffmanTreeRoot = HuffmanTree::heapify(&code, huffmanTables);
}

#endif //HFCOMPRESSOR_FILEIO_CPP