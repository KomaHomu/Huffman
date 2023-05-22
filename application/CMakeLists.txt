cmake_minimum_required(VERSION 3.25)
project(hfcompressor)

set(CMAKE_CXX_STANDARD 17)

add_executable(hfcompressor main.cpp FileStructure.h FileIO.cpp HuffmanTree.cpp HuffmanTree.h FileIO.h command.cpp command.h)
