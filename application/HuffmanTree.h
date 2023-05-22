#ifndef HFCOMPRESSOR_HUFFMANTREE_H
#define HFCOMPRESSOR_HUFFMANTREE_H

#include <vector>
#include "FileStructure.h"

class HuffmanEntry;

class HuffmanTree;

class HuffmanTree {
public:
    struct {
        unsigned int size{};
        char *dataPart{};
        unsigned long frequency{};
        struct HuffmanTree *right{};
        struct HuffmanTree *father{};
        struct HuffmanTree *left{};
    };

    bool operator<(const HuffmanTree *other) const;

    void toTable(const std::vector<bool> &code, std::vector<HuffmanEntry> &huffmanTables) const;

    void flat(std::vector<bool> *code);

    void leaf(std::vector<HuffmanTree*> &leafs);

    static HuffmanTree heapify(std::vector<bool> *code, std::vector<HuffmanEntry> &huffmanTables);
};



struct HuffmanEntry {
    std::vector<bool> code{};
    HuffmanLeaf *huffmanLeaf{};
};
#endif //HFCOMPRESSOR_HUFFMANTREE_H
