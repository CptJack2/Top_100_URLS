#include "zset.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

unsigned long Sharding_Num=2;//120;
int TopK=100;
string file_name="/home/cptjack/Dataset.txt";
unsigned long file_len=0;

void thread_func(unsigned long shard_index){
    unsigned long pos=file_len*shard_index/Sharding_Num;
    fstream ifs(file_name);
    //find a new line from the speicified sharding position
    ifs.seekg(pos);
    while(ifs.peek()!='\n')
        ifs.seekg(++pos);
    zset myzset;
    string str;
    //mind this while expression, it mustn't miss any line
    do{
        ifs>>str;
        myzset.create_or_inc(str);
    }while((pos=ifs.tellg())<=file_len*(shard_index+1)/Sharding_Num);
    auto vec=myzset.pop(TopK);
    fstream ofs("/home/cptjack/res/"+to_string(shard_index)+".txt",ios::out);
    for(auto ele:vec){
        ofs<<ele.first<<endl<<ele.second<<endl;
    }
}
void map_files(){
    fstream ifs(file_name);
    vector<ofstream> ofss(Sharding_Num);
    for(int i=0;i<Sharding_Num;++i)
        ofss[i].open("/home/cptjack/shard/"+to_string(i)+".txt",ios::out);
    hash<string> hasher;
    string str;
    while(!ifs.eof()){
        str.clear();
        ifs>>str;
        if(str.empty())continue;
        ofss[hasher(str)%Sharding_Num]<<str<<endl;
    }
    for(auto& ele:ofss)
        ele.close();
}
void reduce_files(){
    for(int i=0;i<Sharding_Num;++i){
        ifstream ifs;
        ifs.open("/home/cptjack/shard/"+to_string(i)+".txt",ios::in);
        zset myzset;
        string str;
        int total=0;
        while(!ifs.eof()){
            //stupid fucking fstream remain the string the way it was in last cycle when it read a blank line, fuck
            str.clear();
            ifs>>str;
            if(str.empty())continue;
            myzset.create_or_inc(str);
            ++total;
        }
        auto vec=myzset.pop(TopK);
        fstream ofs("/home/cptjack/res/"+to_string(i)+".txt",ios::out);
        for(auto ele:vec)
            ofs<<ele.first<<endl<<ele.second<<endl;
        ofs.close();
    }
}
void merge(){
    typedef pair<string,int> ele_t;
    vector<ele_t> myvec(TopK,ele_t("",0));

    for(int i=0;i<Sharding_Num;++i){
        ifstream ifs("/home/cptjack/res/"+to_string(i)+".txt",ios::in);
        string str;
        int num;
        for(int j=0;j<TopK;++j){
            str.clear();
            num=0;
            ifs>>str>>num;
            if(str.empty())continue;
            int ind_replace=0;
            while(myvec[ind_replace].second>=num)++ind_replace;
            if(ind_replace<TopK) {
                myvec[ind_replace].first = str;
                myvec[ind_replace].second = num;
            }
        }
        ifs.close();
    }
    ofstream ofs("/home/cptjack/final.txt",ios::out);
    for(auto& ele:myvec)
        ofs<<ele.first<<" "<<ele.second<<endl;
    ofs.close();
}
int main(){
    //initiailize
    //get file length
    fstream fs(file_name);
    fs.seekg(0, ios_base::end);
    file_len = fs.tellg();
    fs.close();
//    map_files();
//    reduce_files();
    merge();
    return 0;
}
