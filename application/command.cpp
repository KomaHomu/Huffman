#include "command.h"
#include <cstring>
#include "FileIO.h"
#include "filesystem"
#include <algorithm>
#include <queue>
#include "HuffmanTree.h"

using namespace std;
namespace fs = filesystem;

void Command::compress(char *path, char *compressPath, int wordSize, float splitRate) {
    FileIO fileIO = FileIO();
    fileIO.path = path;
    strcpy(fileIO.pathTempData, path);
    strcat(fileIO.pathTempData, ".tmp");
    fs::create_directories(fs::path(path).parent_path());
    fs::path compress = fs::path(compressPath);
    if (is_regular_file(compress)) {
        fs::current_path(compress.parent_path());
        parseDataCompress(compress, wordSize, splitRate);
    } else {
        fs::current_path(compress.parent_path());
        fs::recursive_directory_iterator pathIterator = fs::recursive_directory_iterator(compress);
        parseDataCompress(pathIterator, wordSize, splitRate);
    }

    // clear temp data
    dict.clear();
    res.clear();
    //

}

void Command::parseDataCompress(fs::recursive_directory_iterator &pathIterator, int wordSize, float splitRate) {
    for (const auto &entry: pathIterator) {
        if (entry.is_regular_file()) {
            fileSize = entry.file_size();
            string pathS = fs::proximate(entry.path());
            vector<unsigned long long> temp = vector<unsigned long long>();
            char *path = (char *) malloc(pathS.size());
            strcpy(path, pathS.c_str());
            lzw_split(path, temp, wordSize, splitRate,1);
            res.push_back(temp);
        }
    }
}

void Command::parseDataCompress(fs::path &entry, int wordSize, float splitRate) {
    fileSize = file_size(entry);
    string pathS = fs::proximate(absolute(entry));
    vector<unsigned long long> temp = vector<unsigned long long>();
    char *path = (char *) malloc(pathS.size());
    strcpy(path, pathS.c_str());
    lzw_split(path, temp, wordSize, splitRate,1);
    res.push_back(temp);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void Command::lzw_split(char *path, vector<unsigned long long> &res1, int wordSize, float splitRate, int time) {
    printf("%s\n",path);
    FILE *file = fopen(path, "rb");

    if (file == nullptr) {
        throw runtime_error("File not found");
    }

    if (wordSize > fileSize) {
        wordSize = (int) fileSize;
    }
    int windowSize = 32;
    string word;

    char *buffer = (char *) malloc(windowSize);
    while (fread(buffer, 1, windowSize, file)) {
        string temp = word + string(buffer, strlen(buffer));
        if (find(dict.begin(), dict.end(), temp) != dict.end()) {
            word = temp;
        } else {
            if (!word.empty()) {
                res1.push_back(distance(dict.begin(), find(dict.begin(), dict.end(), word)));
            }
            if (temp.size() <= wordSize * windowSize)
                dict.push_back(temp);
            word = buffer;
        }
    }
    fclose(file);

    if (!word.empty()) {
        if (find(dict.begin(), dict.end(), word) == dict.end())
            dict.push_back(word);
        res1.push_back(distance(dict.begin(), find(dict.begin(), dict.end(), word)));
    }

    unsigned long long ratio = 0;
    for (unsigned long long i: res1) {
        if (dict.at(i).size() >= wordSize * windowSize)
            ratio += 1;
    }

    if (time > 0) {
        res1.clear();
        lzw_split(path, res1, wordSize, splitRate, time - 1);
    }
}

vector<HuffmanTree> Command::calc_freq_list() {
    vector<HuffmanTree> huffmanTree = vector<HuffmanTree>();
    for (const vector<unsigned long long> &i: res) {
        for (unsigned long long j: i) {
            auto node = find_if(huffmanTree.begin(), huffmanTree.end(),
                                [&](HuffmanTree &huffmanTree) {
                                    return strcmp(huffmanTree.dataPart, dict.at(j).c_str()) == 0;
                                });
            if (node != huffmanTree.end()) {
                HuffmanTree temp = HuffmanTree();
                temp.dataPart = strcpy(temp.dataPart, dict.at(j).c_str());
                temp.frequency = 1;
            } else {
                node->frequency++;
            }
        }
    }
    return huffmanTree;
}

void Command::extract(char *path, char *extractPath) {};

void Command::extract(char *path, char *extractPath, char *extractFile) {};

void Command::examine(char *path) {};

#pragma clang diagnostic pop
