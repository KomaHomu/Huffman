#ifndef HFCOMPRESSOR_FILEIO_H
#define HFCOMPRESSOR_FILEIO_H

#include <vector>
#include <bitset>
#include "FileStructure.h"
#include "HuffmanTree.h"

class FileIO {
public:
    struct {
        char *pathTempData{};
        char *path{};
        FileStructure *fileStructure{};
        HuffmanTree *huffmanTreeRoot{};
    };

    //write
    void writeHeader() const;;

    void generateTreeData() const;

    void addFile(char *path) const;

    void writeBitData(bool bit) const;

    void flush() const;

    //read
    void readHeader() const;

    void treeBuild() const;

private:
    struct {
        FileInside *current{};
        std::bitset<8> *data = new std::bitset<8>();
    };
};

#endif //HFCOMPRESSOR_FILEIO_H
