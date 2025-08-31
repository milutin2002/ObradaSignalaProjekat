//
// Created by milutin on 8/28/25.
//

#ifndef REDUCE_H
#define REDUCE_H
#include <istream>
#include<fstream>
#include <iostream>
using namespace  std;

struct Pixel {
    int r,g,b;
};

static bool nextToken(std::istream& is, std::string& tok);
bool readPGM(std::string &filename, int &width, int &height, int &maxVal, int ***a, std::string&magicByte);
bool readPPM(std::string filename, std::string& magicByte,int &width,int &height,int &maxVal,Pixel ***data);
bool writePGM(std::string  filename, std::string magicByte,int width,int height,int maxVal,int **data);
bool writePPM(std::string  filename,int width,int height,int maxVal,Pixel **data);
int ** downSample(int **data,int width,int height);
Pixel ** downSamplePPM(Pixel **data,int width,int height);
void reduce(std::string& filename);
void rleReduce(string& filename);

#endif //REDUCE_H
