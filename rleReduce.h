//
// Created by milutin on 8/31/25.
//
#include <cstdint>
#include<iostream>
#include <vector>
#include "reduce.h"
using namespace std;
#ifndef RLEREDUCE_H
#define RLEREDUCE_H

vector<pair<uint16_t,uint16_t>> rle_encoding(int **a,int height,int width);
vector<pair<uint16_t,uint16_t>> rle_encoding_ppm(Pixel **a,int height,int width);
void writeToFileRLE(string filename,vector<pair<uint16_t,uint16_t>>&data,string magic,int height,int width,int maxVal);
void rleReduce(string& filename);
void rle_decode(string& filename);

#endif //RLEREDUCE_H
