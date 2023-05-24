#ifndef HFCOMPRESSOR_HUFFMANTREE_H
#define HFCOMPRESSOR_HUFFMANTREE_H

#include <vector>
#include "FileStructure.h"

using namespace std;

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

    void toTable(const vector<bool> &code, vector<HuffmanEntry> &huffmanTables) const;

    void flat(vector<bool> *code) const;

    void leaf(vector<HuffmanTree *> &leafs);

    static HuffmanTree heapify(vector<bool> *code, vector<HuffmanEntry> &huffmanTables);
};


struct HuffmanEntry {
    vector<bool> code{};
    HuffmanLeaf *huffmanLeaf{};
};
#endif //HFCOMPRESSOR_HUFFMANTREE_H
