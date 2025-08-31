#include <iostream>
#include <unordered_map>
#include <string>
#include<queue>
#include <fstream>
#include <sstream>
#include <csignal>
#include <cstdint>

#include "reduce.h"
#include "rleReduce.h"
#include "TreeNode.h"
using namespace std;

unordered_map<char,int>recnik(string filename){
    unordered_map<char,int>map;
    ifstream ifs{filename};

    if(!ifs) {
        return map;
    }
    while(ifs.peek()!=EOF){
        char ch=(char)ifs.get();
        if(map.find(ch)==map.end()){
            map[ch]=0;
        }
        map[ch]++;
    }
    ifs.close();
    return map;
}
int getParent(int i){
    return (i-1)/2;
}
int getLeft(int i){
    return 2*i+1;
}
int getRight(int i){
    return getLeft(i)+1;
}
void heapify(TreeNode **heap,int n,int i){
    int min=i;
    int left= getLeft(i);
    int right=left+1;
    if(left<n && *heap[min]>*heap[left]){
        min=left;
    }
    if(right<n && *heap[min]>*heap[right]){
        min=right;
    }
    if(min!=i){
        swap(heap[min],heap[i]);
        heapify(heap,n,min);
    }
}
TreeNode * getMin(TreeNode **heap,int &n){
    TreeNode *min=heap[0];
    swap(heap[0],heap[n-1]);
    n--;
    int i=0;
    bool hasMin=true;
    while(getLeft(i)<n){
        int next= i;
        int left= getLeft(i);
        if(*heap[left]<*heap[next]){
            next=left;
        }
        int right=left+1;
        if(right<n && *heap[right]<*heap[next]){
            next=right;
        }
        if(next!=i){
            swap(heap[i],heap[next]);
            i=next;
        }
        else{
            return min;
        }
    }
    return min;
}

void insert(TreeNode **heap,int &n,TreeNode *t){
    heap[n]=t;
    n++;
    int i=n-1;
    while(i>0){
        int parent= getParent(i);
        if(*heap[parent]>*heap[i]){
            swap(heap[parent],heap[i]);
            i=parent;
        }
        else{
            return;
        }
    }
}
TreeNode** makeHeap(unordered_map<char,int>map){
    TreeNode **heap=new TreeNode*[map.size()];
    int br=0;
    auto it=map.begin();
    while(it!=map.end()){
        heap[br++]=new TreeNode(it->first,it->second);
        it++;
    }
    int n=map.size();
    for (int i = getParent(n-1); i >=0 ; i--) {
        heapify(heap,n,i);
    }
    return heap;
}
TreeNode*makeTree(unordered_map<char,int>map){
    int n=map.size();
    TreeNode **heap=makeHeap(map);
    while(n>1){
        TreeNode *min1= getMin(heap,n);
        TreeNode *min2= getMin(heap,n);
        TreeNode *t=new TreeNode(min1,min2);
        insert(heap,n,t);
    }
    TreeNode *tree=heap[0];
    delete[]heap;
    return tree;
}
unordered_map<char,string>makeCodes(TreeNode *t){
    if(t== nullptr){
        return unordered_map<char,string>();
    }
    queue<pair<TreeNode*,string>>q;
    unordered_map<char,string>codes;
    q.push(make_pair(t,""));
    while(!q.empty()){
        auto par=q.front();
        q.pop();
        TreeNode *curr=par.first;
        string  code=par.second;
        if(curr->isLeaf()){
            codes[curr->value]=code;
        }
        else{
            if(curr->levi!= nullptr){
                q.push(make_pair(curr->levi,code+"0"));
            }
            if(curr->desni!= nullptr){
                q.push(make_pair(curr->desni,code+"1"));
            }
        }
    }
    return codes;
}
string readAllTheText(string& filename){
    stringstream strStream;
    ifstream ifs{filename};
    if(!ifs){
        return "";
    }
    strStream<<ifs.rdbuf();
    ifs.close();
    return strStream.str();
}
void compress(string& filename){
    TreeNode *t= makeTree(recnik(filename));
    auto codes= makeCodes(t);
    ofstream of{filename+".dir"};
    ofstream bin{filename+".pghm",ios::binary};
    int bit=7;
    u_int8_t br=0;
    string text= readAllTheText(filename);
    of<<text.size()<<endl;
    for(auto code:codes){
        if(code.first=='\n'){
            of<<"\\n"<<" "<<code.second<<endl;
        }
        else if(code.first==' '){
            of<<"blank"<<" "<<code.second<<endl;
        }
        else {
            of << code.first << " " << code.second << endl;
        }
    }
    for (int i = 0; i <text.size(); ++i) {
        for (int j = 0; j <codes[text[i]].size() ; ++j) {
            if(bit==-1){
                bin.write(reinterpret_cast<char*>(&br),sizeof (br));
                br=0;
                bit=7;
            }
            int add=codes[text[i]][j]-'0';
            int mask=1<<bit;
            if(add==1){
                br=br|mask;
            }
            else{
                br=br&(~mask);
            }
            bit--;
        }
    }
    br=br>>(bit+1);
    bin<<br;
    bin.close();
    of.close();
    delete t;
}

void decompress(string& output){
    if(!(output.substr(output.size()-5,5)==".pghm")) {
        cout<<"Bad file format"<<endl;
        return;
    }
    ifstream ifs{output,ios::binary};
    ifstream diri{output.substr(0,output.size()-5)+".dir"};
    unordered_map<string,string>map;
    int n;
    diri>>n;
    int kar=0;
    while(!diri.eof()){
        string kar,code;
        diri>>kar>>code;
        cout<<kar<<" "<<code<<endl;
        map[code]=kar;
    }
    unsigned char uint8;
    string res="";
    ofstream off{output.substr(0,output.size()-5)};
    while(!ifs.eof() && kar<n){
        ifs.read(reinterpret_cast<char *>(&uint8),sizeof (uint8));
        for (int i = 7; i >=0 && kar<n; i--) {
            int br=((1<<i)&uint8)==(1<<i);
            if(br==0){
                res+="0";
            }
            else{
                res+="1";
            }
            if(map.find(res)!=map.end()){
                kar++;
                if(map[res]=="blank"){
                    off<<" ";
                }
                else if(map[res]=="\\n"){
                    off<<endl;
                }
                else {
                    off << map[res];
                }
                res="";
            }
        }
    }
    ifs.close();
    diri.close();
    off.close();
}
void encodeFile(string &filename) {
    int br;
    cout<<"Enter number"<<endl;
    cout<<"1: reduce by block"<<endl;
    cout<<"2: reduce with rle"<<endl;
    cout<<"3: reduce with huffman"<<endl;
    cin>>br;
    switch (br) {
        case 1:
            reduce(filename);break;
        case 2:
            rleReduce(filename);break;
        case 3:
            compress(filename);break;
        default: cout<<"Not valid option"<<endl;
    }
}
void decodeFile(string& filename) {
    int br;
    cout<<"Enter number"<<endl;
    cout<<"1: decode rle"<<endl;
    cout<<"2: decode huffman"<<endl;
    if(br==1) {
        rle_decode(filename);
    }
    else {
        decompress(filename);
    }
}
int main() {
    string filename;
    cout<<"Enter filename"<<endl;
    cin>>filename;
    int br;
    cout<<"Enter number"<<endl;
    cout<<"1: encode"<<endl;
    cout<<"2: decode"<<endl;
    cin>>br;
    if(br==1) {
        encodeFile(filename);
    }
    else {
        decodeFile(filename);
    }
    return 0;
}
