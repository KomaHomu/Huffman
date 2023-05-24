#ifndef HFCOMPRESSOR_COMMAND_H
#define HFCOMPRESSOR_COMMAND_H

#include <filesystem>
#include <queue>
#include <map>
#include "FileIO.h"
#include <cstring>

using namespace std;
namespace fs = filesystem;

class Command {

public:

    static void compress(char *path, char *compressPath, int wordSize, float splitRate);

    static void extract(char *path, char *extractPath);

    static void extract(char *path, char *extractPath, char *extractFile);

    static void examine(char *path);

private:
    static void parseDataCompress(fs::recursive_directory_iterator &pathIterator, int wordSize, float splitRate);

    static void parseDataCompress(fs::path &entry, int wordSize, float splitRate);

    static vector<HuffmanTree> calc_freq_list();

    static void lzw_split(char *path, vector<unsigned long long> &res1,
                          int wordSize, float splitRate, int time);
};

static unsigned long long fileSize = 0;
static vector<string> dict = vector<string>();
static vector<vector<unsigned long long>> res = vector<vector<unsigned long long>>();
#endif //HFCOMPRESSOR_COMMAND_H
