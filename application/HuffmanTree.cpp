#include <vector>
#include <stdexcept>
#include "FileStructure.h"
#include "HuffmanTree.h"

#ifndef HFCOMPRESSOR_HUFFMANTREE_CPP
#define HFCOMPRESSOR_HUFFMANTREE_CPP

void HuffmanTree::toTable(const std::vector<bool> &code, std::vector<HuffmanEntry> &huffmanTables) const {
    if (this->left != nullptr) {
        std::vector<bool> code_copy = std::vector<bool>(code);
        code_copy.push_back(false);
        this->left->toTable(code_copy, huffmanTables);
    }
    if (this->right != nullptr) {
        std::vector<bool> code_copy = std::vector<bool>(code);
        code_copy.push_back(true);
        this->right->toTable(code_copy, huffmanTables);
    }
    if (this->left == nullptr && this->right == nullptr) {
        auto *entry = new HuffmanEntry();
        auto *leaf = new HuffmanLeaf();
        leaf->size = this->size;
        leaf->dataPart = this->dataPart;
        entry->code = code;
        entry->huffmanLeaf = leaf;
        huffmanTables.push_back(*entry);
    }
}

bool HuffmanTree::operator<(const HuffmanTree *other) const {
    return this->frequency < other->frequency;
}

void HuffmanTree::flat(std::vector<bool> *code){
    if (this->left != nullptr) {
        code->push_back(false);
        this->left->flat(code);
    }
    if (this->right != nullptr) {
        code->push_back(true);
        this->right->flat(code);
    }
}

void HuffmanTree::leaf(std::vector<HuffmanTree*> &leafs) {
    if (this->left != nullptr) {
        this->left->leaf(leafs);
    }
    if (this->right != nullptr) {
        this->right->leaf(leafs);
    }
    if (this->left == nullptr && this->right == nullptr) {
        leafs.push_back(this);
    }
}

HuffmanTree HuffmanTree::heapify(std::vector<bool> *code, std::vector<HuffmanEntry> &huffmanTables) {
    HuffmanTree node = HuffmanTree();
    if (code->empty()) {
        return node;
    }
    HuffmanTree *current = &node;
    for (bool i: *code) {
        if (!i) {
            current->left = new HuffmanTree();
            current->left->father = current;
            current = current->left;

        } else {
            current = current->father;
            while (current->right != nullptr) {
                current = current->father;
            }
            current->right = new HuffmanTree();
            current->right->father = current;
            current = current->right;
        }
    }
    std::vector<HuffmanTree*> leafs = std::vector<HuffmanTree*>();
    node.leaf(leafs);
    if (leafs.size() != huffmanTables.size()) {
        throw std::runtime_error("HuffmanTree::heapify: leafs.size() != huffmanTables.size()");
    }
    for (int i = 0; i < leafs.size(); ++i) {
        leafs[i]->size = huffmanTables[i].huffmanLeaf->size;
        leafs[i]->dataPart = huffmanTables[i].huffmanLeaf->dataPart;
    }
    return node;
}

#endif //HFCOMPRESSOR_HUFFMANTREE_CPP