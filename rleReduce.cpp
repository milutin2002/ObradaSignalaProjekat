//
// Created by milutin on 8/31/25.
//
#include "rleReduce.h"
#include "reduce.h"
#include<fstream>

vector<pair<uint16_t,uint16_t>> rle_encoding(int **a,int height,int width) {
    vector<pair<uint16_t,uint16_t>>res;
    int current=-1;
    int count=0;
    for (int i = 0; i < height; ++i) {
        for(int j=0;j<width;j++) {
            if(a[i][j]==current && count<65535) {
                count++;
            }
            else {
                res.emplace_back(count,current);
                current=a[i][j];
                count=1;
            }
        }
    }
    res.emplace_back(count,current);
    return res;
}
vector<pair<uint16_t,uint16_t>> rle_encoding_ppm(Pixel **a,int height,int width) {
    vector<pair<uint16_t,uint16_t>>res;
    int current=-1;
    int count=0;
    for (int i = 0; i < height; ++i) {
        for(int j=0;j<width;j++) {
            int *b=(int *)&a[i][j];
            for(int k=0;k<3;k++) {
                if(b[k]==current && count<65535) {
                    count++;
                }
                else {
                    res.emplace_back(count,current);
                    current=b[k];
                    count=1;
                }
            }

        }
    }
    res.emplace_back(count,current);
    return res;
}
void writeToFileRLE(string filename,vector<pair<uint16_t,uint16_t>>&data,string magic,int height,int width,int maxVal) {
    ofstream ofs{filename};
    ofs<<magic<<"\n";
    ofs<<height<<" "<<width<<"\n";
    if(magic!="P1") {
        ofs<<maxVal<<"\n";
    }
    for (int i = 0; i < data.size(); ++i) {
        ofs<<data[i].first<<" "<<data[i].second<<" ";
    }
    ofs.close();
}

void rleReduce(string& filename) {
    int width,height,maxVal;
    int **a;
    int ***b;
    string magic;
    if(filename.substr(filename.size()-4,4)!=".ppm") {
        bool readSuc=readPGM(filename,width,height,maxVal,&a,magic);
        if(!readSuc) {
            cerr<<"Unable to read file";
            return;
        }
        auto data=rle_encoding(a,height,width);
        filename=filename+".rle";
        writeToFileRLE(filename,data,magic,height,width,maxVal);
    }
    else {
        Pixel **matrix;
        bool readSuc=readPPM(filename,magic,width,height,maxVal,&matrix);
        if(!readSuc) {
            cerr<<"Unable to read file";
            return;
        }
        auto data=rle_encoding_ppm(matrix,height,width);
        filename=filename+".rle";
        writeToFileRLE(filename,data,magic,height,width,maxVal);
    }

}
void rle_decode(string& filename) {
    if(filename.substr(filename.size()-4,4)!=".rle") {
        return;
    }
    string file=filename.substr(0,filename.size()-4);
    ifstream ifs{filename};
    ofstream ofs{file};
    string magic;
    int width,height,maxVal;
    ifs>>magic;
    ifs>>width>>height;
    ofs<<magic<<"\n";
    ofs<<height<<" "<<width;
    if(magic!="P1") {
        ifs>>maxVal;
        ofs<<"\n"<<maxVal;
    }
    int n=height*width;
    if(magic=="P3") {
        n*=3;
        width*=3;
    }
    int j=0;
    while(!ifs.eof()) {
        int count,br;
        ifs>>count>>br;
        for(int i=0;i<count;i++) {
            if(j%width) {
                ofs<<" ";
            }
            else if(j!=n){
                ofs<<"\n";
            }
            ofs<<br;
            j++;
        }
    }
    ifs.close();
    ofs.close();
}