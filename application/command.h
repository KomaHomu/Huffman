#ifndef HFCOMPRESSOR_COMMAND_H
#define HFCOMPRESSOR_COMMAND_H

#include <filesystem>
#include <queue>
#include "FileIO.h"
namespace fs = std::filesystem;
class Command {

public:

    static void compress(char *path, char *compressPath);

    static void extract(char *path, char *extractPath);

    static void extract(char *path, char *extractPath, char *extractFile);

    static void examine(char *path);

private:
    static void parseData(FileIO &fileIO, fs::recursive_directory_iterator &pathIterator);


    static std::priority_queue<HuffmanTree> calc_freq_list(FileIO &fileIO);
};

static unsigned long long size = 0;
#endif //HFCOMPRESSOR_COMMAND_H
