#include <iostream>
#include <vector>
#include <algorithm>
#include "filesystem"
#include <cstring>
#include "HuffmanTree.h"

using namespace std;
namespace fs = filesystem;
static vector<string> dict = vector<string>();
static vector<vector<unsigned long long>> res = vector<vector<unsigned long long>>();
static unsigned long long fileSize = 0;

static void lzw_split(char *path, vector<unsigned long long> &res1, int wordSize) {
    printf("%s\n",path);
    FILE *file = fopen(path, "rb");

    if (file == nullptr) {
        throw runtime_error("File not found");
    }
    int windowSize = 4096;
    if (fileSize < (unsigned long long ) wordSize * windowSize) {
        wordSize = (int) fileSize;
        windowSize = (int) fileSize;
    }
    string word;

    while (true) {
        if (fileSize < windowSize) {
            windowSize = (int) fileSize;
        }
        if (fileSize == 0) {
            break;
        }
        char *buffer = (char *) malloc(windowSize);
        fread(buffer, 1, windowSize, file);
        fileSize -= windowSize;
        string temp = word + string(buffer);
        if (find(dict.begin(), dict.end(), temp) != dict.end()) {
            word = temp;
        } else {
            if (!word.empty()) {
                res1.push_back(distance(dict.begin(), find(dict.begin(), dict.end(), word)));
            }
            if (temp.size() <= (unsigned long long) wordSize * windowSize)
                dict.push_back(temp);
            word = string(buffer, strlen(buffer));
        }
    }
    fclose(file);
    if (!word.empty()) {
        if (find(dict.begin(), dict.end(), word) == dict.end())
            dict.push_back(word);
        res1.push_back(distance(dict.begin(), find(dict.begin(), dict.end(), word)));
    }
}

static void parseDataCompress(fs::recursive_directory_iterator &pathIterator, int wordSize) {
    vector<char*> paths = vector<char*>();
    string pathS;
    for (auto &entry: pathIterator) {
        if (entry.is_regular_file()) {
            fileSize = entry.file_size();
            pathS = entry.path().u8string();
            pathS = pathS.replace(0,fs::current_path().u8string().size()+1, "");
            vector<unsigned long long> temp = *new vector<unsigned long long>();
            char *path = (char *) malloc(pathS.size());
            strcpy(path, pathS.c_str());
            paths.push_back(path);
            lzw_split(path, temp, wordSize);
            res.push_back(temp);
        }
    }
    for (unsigned long long i = 0; i < paths.size(); i++) {
        lzw_split(paths.at(i),res.at(i),wordSize);
    }
}

static void parseDataCompress(fs::path &entry, int wordSize) {
    fileSize = file_size(entry);
    string pathS = entry.filename();
    vector<unsigned long long> temp = vector<unsigned long long>();
    char *path = (char *) malloc(pathS.size());
    strcpy(path, pathS.c_str());
    lzw_split(path, temp, wordSize);
    lzw_split(path, temp, wordSize);
    res.push_back(temp);
}

void compress(char *path, char *compressPath, int wordSize) {
    fs::create_directories(fs::path(path).parent_path());
    fs::path compress = fs::path(compressPath);
    if (is_regular_file(compress)) {
        fs::current_path(compress.parent_path());
        parseDataCompress(compress, wordSize);
    } else {
        fs::current_path(compress.parent_path());
        fs::recursive_directory_iterator pathIterator = fs::recursive_directory_iterator(compress);
        parseDataCompress(pathIterator, wordSize);
    }
}
vector<HuffmanTree> calc_freq_list() {
    vector<HuffmanTree> huffmanTree = vector<HuffmanTree>();
    for (const vector<unsigned long long> &i: res) {
        for (unsigned long long j: i) {
            std::vector<HuffmanTree>::iterator node = find_if(huffmanTree.begin(), huffmanTree.end(),
                                [&](HuffmanTree &huffmanTree) {
                                    return strcmp(huffmanTree.dataPart, dict.at(j).c_str()) == 0;
                                });
            if (node == huffmanTree.end()) {
                HuffmanTree temp = HuffmanTree();
                temp.size = strlen(dict.at(j).c_str())+1;
                temp.dataPart = (char*) malloc(temp.size);
                strcpy(temp.dataPart, dict.at(j).c_str());
                temp.frequency = 1;
                huffmanTree.push_back(temp);
            } else {
                node->frequency++;
            }
        }
    }
    return huffmanTree;
}
int main() {
    compress("/media/phambathang/LinuxExtend/Jetbrain/apps/apps/CLion/config/scratches/scratch.hf",
             "/home/phambathang/Desktop/practical-face-detection",
             4);
    int t =0;
    for (HuffmanTree i:calc_freq_list()){
        if (i.frequency > 1){
            t ++;
        }
    }
    printf("%d",t);
}
