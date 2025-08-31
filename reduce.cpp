//
// Created by milutin on 8/28/25.
//
#include "reduce.h"

bool nextToken(std::istream& is, std::string& tok) {
    tok.clear();
    char c;
    // skip whitespace & comments
    while (is.get(c)) {
        if (isspace((unsigned char)c)) continue;
        if (c == '#') {
            std::string dummy;
            getline(is, dummy);    // preskoci do kraja linije
            continue;
        }
        tok.push_back(c);
        break;
    }
    if (tok.empty()) return false;
    // citaj do sledeceg whitespace/comment
    while (is.get(c)) {
        if (isspace((unsigned char)c)) break;
        if (c == '#') { is.unget(); break; }
        tok.push_back(c);
    }
    return true;
}

bool readPGM(string &filename, int &width, int &height, int &maxVal, int ***a,string&magicByte) {
    ifstream ifs(filename); // ASCII varijante: tekstualno je OK
    if (!ifs.is_open()) {
        return false;
    }
    if (!nextToken(ifs, magicByte)) {
        cerr << "Bad header\n";
        return false;
    }

    string magic=magicByte;
    // Podrzimo P1 (PBM ASCII), P2 (PGM ASCII), P3 (PPM ASCII)
    if (magic != "P1" && magic != "P2" && magic != "P3") {
        cerr << "Only ASCII PNM (P1/P2/P3) supported here\n";
        return false;
    }

    // sirina, visina
    string tok;
    if (!nextToken(ifs, tok)) return false;
    width = stoi(tok);
    if (!nextToken(ifs, tok)) return false;
    height = stoi(tok);

    // maxVal: postoji za P2/P3, ne postoji za P1
    if (magic == "P1") {
        maxVal = 1;
    } else {
        if (!nextToken(ifs, tok)) return false;
        maxVal = stoi(tok);
        if (maxVal <= 0) { cerr << "Bad maxVal\n"; return false; }
    }

    // Alokacija matrice (int**)
    (*a) = new int*[height];
    for (int i = 0; i < height; i++) {
        (*a)[i] = new int[width];
    }

    // Citanje piksela
    if (magic == "P1") {
        // PBM ASCII: vrednosti 0 ili 1
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (!nextToken(ifs, tok)) { cerr << "Unexpected EOF\n"; return false; }
                int v = stoi(tok);
                if (v != 0 && v != 1) { cerr << "PBM expects 0/1\n"; return false; }
                (*a)[i][j] = v; // ostaje 0/1
            }
        }
    } else if (magic == "P2") {
        // PGM ASCII: jedna vrednost po pikselu u [0..maxVal]
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (!nextToken(ifs, tok)) { cerr << "Unexpected EOF\n"; return false; }
                int v = stoi(tok);
                //if (v < 0 || v > maxVal) { cerr << "PGM value out of range\n"; return false; }
                (*a)[i][j] = v;
            }
        }
    }
    ifs.close();
    return true;
}



bool writePGM(string  filename,string magicByte,int width,int height,int maxVal,int **data) {
    ofstream ofs{filename};
    if(!ofs.is_open()) {
        return false;
    }
    ofs<<magicByte<<"\n"<<width<<" "<<height<<"\n"<<maxVal<<"\n";
    for(int i=0;i<height;i++) {
        for (int j = 0; j < width; ++j) {
            ofs<<data[i][j]<<" ";
        }
        ofs<<"\n";
    }
    ofs.close();
    return true;
}

bool readPPM(string filename,string& magicByte,int &width,int &height,int &maxVal,Pixel ***data) {
    ifstream ifs(filename); // ASCII varijante: tekstualno je OK
    if (!ifs.is_open()) {
        return false;
    }
    if (!nextToken(ifs, magicByte)) {
        cerr << "Bad header\n";
        return false;
    }

    string magic=magicByte;
    // Podrzimo P1 (PBM ASCII), P2 (PGM ASCII), P3 (PPM ASCII)
    if (magic != "P3") {
        cerr << "Only ASCII PNM (P1/P2/P3) supported here\n";
        return false;
    }
    string token;
    if (!nextToken(ifs, token)) return false;
    width = stoi(token);
    if (!nextToken(ifs, token)) return false;
    height = stoi(token);
    if(!nextToken(ifs,token)) {
        cerr<<"Unexpected eof"<<endl;
        return false;
    }
    maxVal=stoi(token);
    (*data)=new Pixel*[height];
    for (int i = 0; i < height; ++i) {
        (*data)[i]=new Pixel[width];
        for(int j=0;j<width;j++) {
            int r,g,b;
            if(!nextToken(ifs,token)) {
                cerr<<"Unexpected eof"<<endl;
                return false;
            }
            r=stoi(token);
            if(!nextToken(ifs,token)) {
                cerr<<"Unexpected eof"<<endl;
                return false;
            }
            g=stoi(token);
            if(!nextToken(ifs,token)) {
                cerr<<"Unexpected eof"<<endl;
                return false;
            }
            b=stoi(token);
            (*data)[i][j]={r,g,b};
        }
    }
    return true;
}

bool writePPM(string  filename,int width,int height,int maxVal,Pixel **data) {
    ofstream ofs{filename};
    if(!ofs.is_open()) {
        return false;
    }
    ofs<<"P3\n"<<width<<" "<<height<<"\n"<<maxVal<<"\n";
    for(int i=0;i<height;i++) {
        for (int j = 0; j < width; ++j) {
            ofs<<data[i][j].r<<" "<<data[i][j].g<<" "<<data[i][j].b<<" ";
        }
        ofs<<"\n";
    }
    ofs.close();
    return true;
}

int ** downSample(int **data,int width,int height) {
    int **a=new int*[height];
    for (int i=0;i<height;i++) {
        a[i]=new int[width];
        for(int j=0;j<width;j++) {
            a[i][j]=(data[2*i][2*j]+data[2*i][2*j+1]+data[2*i+1][2*j]+data[2*i+1][2*j+1])/4;
        }
    }
    return a;
}

Pixel ** downSamplePPM(Pixel **data,int width,int height) {
    Pixel **a=new Pixel*[height];
    for (int i=0;i<height;i++) {
        a[i]=new Pixel[width];
        for(int j=0;j<width;j++) {
            a[i][j].r=(data[2*i][2*j].r+data[2*i][2*j+1].r+data[2*i+1][2*j].r+data[2*i+1][2*j+1].r)/4;
            a[i][j].g=(data[2*i][2*j].g+data[2*i][2*j+1].g+data[2*i+1][2*j].g+data[2*i+1][2*j+1].g)/4;
            a[i][j].b=(data[2*i][2*j].b+data[2*i][2*j+1].b+data[2*i+1][2*j].b+data[2*i+1][2*j+1].b)/4;
        }
    }
    return a;
}

void reduce(string& filename) {
    int width,height,maxVal;
    int **a;
    string magic;
    if(filename.substr(filename.size()-4,4)!=".ppm") {
        bool readSuc=readPGM(filename,width,height,maxVal,&a,magic);
        if(!readSuc) {
            cerr<<"You can't read file"<<endl;
            return;
        }
        int ** data=downSample(a,width/2,height/2);
        writePGM(filename+".red",magic,width/2,height/2,maxVal,data);
    }
    else {
        string magic;
        Pixel **pixels;
        bool readSuc=readPPM(filename,magic,width,height,maxVal,&pixels);
        if(!readSuc) {
            cerr<<"You can't read file"<<endl;
            return;
        }
        Pixel ** data=downSamplePPM(pixels,width/2,height/2);
        writePPM(filename+".red",width/2,height/2,maxVal,data);
    }

}