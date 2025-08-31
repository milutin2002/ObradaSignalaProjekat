cmake_minimum_required(VERSION 3.27)
project(hufman)

set(CMAKE_CXX_STANDARD 17)

add_executable(hufman main.cpp
        reduce.h
        reduce.cpp
        rleReduce.h
        rleReduce.cpp
        TreeNode.cpp
        TreeNode.h)
