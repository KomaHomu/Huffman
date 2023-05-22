#include "command.h"
#include <cstring>
#include "FileIO.h"
#include "filesystem"
#include <queue>

namespace fs = std::filesystem;

void Command::compress(char *path, char *compressPath) {
    FileIO  fileIO = FileIO();
    fileIO.path = path;
    strcpy(fileIO.pathTempData, path);
    strcat(fileIO.pathTempData, ".tmp");
    fs::create_directories(fs::path(fileIO.path).parent_path());
    fs::current_path(fs::path(path).parent_path());
    fs::recursive_directory_iterator pathIterator = fs::recursive_directory_iterator(fs::current_path());
    parseData(fileIO, pathIterator);
}
void Command::extract(char *path, char *extractPath){};
void Command::extract(char *path, char *extractPath, char *extractFile){};
void Command::examine(char *path){};

void Command::parseData(FileIO &fileIO, fs::recursive_directory_iterator &pathIterator) {
    FILE* file = fopen(fileIO.pathTempData,"wb");
    char buffer;
    for (const auto &entry : pathIterator) {
        if (entry.is_regular_file()) {
            FILE* file_current = fopen(entry.path().c_str(),"rb");
            while (fread(&buffer,1,1,file_current)){
                fwrite(&buffer,1,1,file);
                size++;
            }
            fclose(file_current);
        }
    }
}

std::priority_queue<HuffmanTree> Command::calc_freq_list(FileIO &fileIO){
    std::priority_queue<HuffmanTree> freq_list;
    FILE* file = fopen(fileIO.pathTempData,"rb");
    char buffer;
    while (fread(&buffer,1,1,file)){

    }
}
