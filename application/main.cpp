#include <cstdio>
#include <cstring>
#include "command.h"

int main(int argc, char *argv[]){
    const char* help="---HF Compress---\n"
               "Agruments:\n"
               "-c filename.hf [file/folder]: compress file or folder\n"
               "-r file.hf: get list file inside file.hf\n"
               "-x file.hf [path]: extract file or folder to path\n"
               "-x file.hf [path] [file]: extract file to path\n";

    if (argc < 3){
        printf("%s",help);
    }
    else if (strcmp(argv[1],"-c") == 0){
        printf("Compressing %s to %s",argv[2],argv[3]);
        Command::compress(argv[2],argv[3]);
    }
    else if (strcmp(argv[1],"-r") == 0){
        printf("Getting list file inside %s",argv[2]);
        Command::examine(argv[2]);
    }
    else if (strcmp(argv[1],"-x") == 0){
        if (argc == 4){
            printf("Extracting %s to %s",argv[2],argv[3]);
            Command::extract(argv[2],argv[3]);
        }
        else if (argc == 5){
            printf("Extracting %s from %s to %s", argv[4],argv[2],argv[3]);
            Command::extract(argv[2],argv[3],argv[4]);
        }
    }
    else{
        printf("%s",help);
    }
}