#ifndef HFCOMPRESSOR_FILEIO_H
#define HFCOMPRESSOR_FILEIO_H

#include <vector>
#include <bitset>
#include "FileStructure.h"
#include "HuffmanTree.h"

using namespace std;

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
        bitset<8> *data = new bitset<8>();
    };
};

#endif //HFCOMPRESSOR_FILEIO_H
