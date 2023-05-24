#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "command.h"

int main(int argc, char *argv[]) {
    const char *help = "---HF Compress---\n"
                       "Agruments:\n"
                       "-c filename.hf [file/folder]: compress file or folder\n"
                       "-c filename.hf [file/folder] [word size] [split rate]: compress file or folder with word size (integer) and split rate (0.1..0.9)\n"
                       "-r file.hf: get list file inside file.hf\n"
                       "-x file.hf [path]: extract file or folder to path\n"
                       "-x file.hf [path] [file]: extract file to path\n";

    if (strcmp(argv[1], "-c") == 0) {
        if (argc == 3) {
            printf("Compressing %s to %s", argv[3], argv[2]);
            Command::compress(argv[3], argv[2], 4, 0.9);
        } else if (argc == 5) {
            try {
                int wordSize = strtol(argv[4], nullptr, 10);
                float splitRate = strtod(argv[5], nullptr);
                printf("Compressing %s to %s with word size %s and split rate %s", argv[3], argv[2], argv[4], argv[5]);
                Command::compress(argv[3], argv[2], wordSize, splitRate);
            } catch (exception &e) {
                printf("Invalid argument");
            }
        } else {
            printf("%s", help);
        }
    } else if (strcmp(argv[1], "-r") == 0) {
        printf("Getting list file inside %s", argv[2]);
        Command::examine(argv[2]);
    } else if (strcmp(argv[1], "-x") == 0) {
        if (argc == 4) {
            printf("Extracting %s to %s", argv[2], argv[3]);
            Command::extract(argv[2], argv[3]);
        } else if (argc == 5) {
            printf("Extracting %s from %s to %s", argv[4], argv[2], argv[3]);
            Command::extract(argv[2], argv[3], argv[4]);
        } else {
            printf("%s", help);
        }
    } else {
        printf("%s", help);
    }
}