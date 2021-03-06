#include "zset.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <thread>
#include <mutex>
#include <math.h>

using namespace std;

const int core_num=4;
const unsigned long Sharding_Num=100;
const int TopK=100;
const string file_name="../data/Data.txt";
const long mem_lim=1*1024*1024*1024;

struct pended_t{
    int index;
    int sec_shard;
    pended_t(int i,int s):index(i),sec_shard(s){}
};

unsigned long file_len=0;
vector<thread> threads;
vector<pended_t> pended_shards;
vector<ofstream> map_ofss(Sharding_Num);

void map_files() {
    ifstream ifs(file_name);
    string str;
    hash<string> hasher;
    //open output streams
    for (int i = 0; i < Sharding_Num; ++i)
        map_ofss[i].open("../data/shard_" + to_string(i) + ".txt", ios::out);
    while (ifs >> str) {
        int index = hasher(str) % Sharding_Num;
        map_ofss[index] << str << endl;
    }
    ifs.close();
}
void reduce_files(int thread_index){
    for(int i=thread_index;i<Sharding_Num;i+=core_num){
        ifstream ifs;
        ifs.open("../data/shard_"+to_string(i)+".txt",ios::in);
        //get shard size, if over mem_lim, pend it for further operation
        ifs.seekg(0,ios::end);
        long fsize=ifs.tellg();
        if(fsize>mem_lim) {
            pended_shards.push_back(pended_t(i,ceil(double(fsize)/mem_lim)));
            ifs.close();
            continue;
        }
        ifs.close();
        ifs.open("../data/shard_"+to_string(i)+".txt",ios::in);
        zset myzset;
        string str;
        while(ifs>>str)
            myzset.create_or_inc(str);
        ifs.close();

        auto vec=myzset.pop(TopK);
        fstream ofs("../data/res_"+to_string(i)+".txt",ios::out);
        for(auto ele:vec)
            ofs<<ele.first<<endl<<ele.second<<endl;
        ofs.close();
    }
}
void multi_thread_reduce(){
    threads.clear();
    for (int i = 0; i < core_num; ++i)
        threads.emplace_back(reduce_files, i);
    //wait for the mapping threads to end and close the resources
    for (auto &t:threads)t.join();
    threads.clear();
}
void merge(){
    typedef pair<string,int> ele_t;
    list<ele_t> mylist;

    for(int i=0;i<Sharding_Num;++i){
        ifstream ifs("../data/res_"+to_string(i)+".txt",ios::in);
        string str;
        int num;
        for(int j=0;j<TopK;++j){
            str.clear();
            num=0;
            ifs>>str>>num;
            if(str.empty())continue;
            auto it=mylist.begin();
            while(it!=mylist.end() && (*it).second>=num)
                ++it;
            mylist.insert(it,ele_t(str,num));
            if(mylist.size()>TopK)
                mylist.pop_back();
        }
        ifs.close();
    }
    
    ofstream ofs("../data/final.txt",ios::out);
    for(auto& ele:mylist)
        ofs<<ele.first<<" "<<ele.second<<endl;
    ofs.close();
}
void reduce_pended_shard(int thread_index){
    int total_shards_done=0;
    for(int i=0;i<pended_shards.size();++i){
        for(int j=0;j<pended_shards[i].sec_shard;++j){
            if((total_shards_done+j+1)%core_num!=thread_index)continue;
            ifstream ifs("../data/shard_"+to_string(pended_shards[i].index)+"_"+
                to_string(j)+".txt");
            string str;
            ofstream ofs("../data/res_"+to_string(pended_shards[i].index)+"_"+
                to_string(j)+".txt");
            zset zset1;
            while(ifs>>str)
                zset1.create_or_inc(str);
            auto vec=zset1.pop(zset1.size());
            for(auto& ele:vec)
                ofs<<ele.first<<endl<<ele.second<<endl;
            ofs.close();
        }
        total_shards_done+=pended_shards[i].sec_shard;
    }
}
void merge_pended_shard(int thread_index){
    for(int i=thread_index;i<pended_shards.size();i+=core_num) {
        zset zset1;
        for(int j=0;j<pended_shards[i].sec_shard;++j){
            ifstream ifs("../data/res_"+to_string(pended_shards[i].index)+"_"+
                to_string(j)+".txt");
            string str;
            int num;
            while(ifs>>str>>num)
                zset1.create_or_inc(str,num);
        }
        ofstream ofs("../data/res_"+to_string(pended_shards[i].index)+".txt");
        auto vec=zset1.pop(zset1.size());
        for(auto& ele:vec)
            ofs<<ele.first<<" "<<ele.second<<endl;
        ofs.close();
    }
}
void multi_thread_reduce_pended_shard(){
    threads.clear();
    for (int i = 0; i < core_num; ++i)
        threads.emplace_back(reduce_pended_shard, i);
    for (auto &t:threads)t.join();
    threads.clear();
}
void multi_thread_merge_pended_shard(){
    threads.clear();
    for (int i = 0; i < core_num; ++i)
        threads.emplace_back(merge_pended_shard, i);
    for (auto &t:threads)t.join();
    threads.clear();
}
void split_pended_shard(){
    for(int i=0;i<pended_shards.size();++i){
        ifstream ifs("../data/shard_"+to_string(pended_shards[i].index)+".txt");
        int sec_index=0;
        ofstream ofs("../data/shard_"+to_string(pended_shards[i].index)+"_"+
            to_string(sec_index)+".txt");
        string str;
        while(ifs>>str){
            ofs<<str<<endl;
            if(ifs.tellg()>=mem_lim*(sec_index+1)){
                ofs.close();
                ++sec_index;
                ofs.open("../data/shard_"+to_string(pended_shards[i].index)+"_"+
                    to_string(sec_index)+".txt");
            }
        }
        ofs.close();
    }
}
int main(){
    //get file length
    fstream fs(file_name);
    fs.seekg(0, ios_base::end);
    file_len = fs.tellg();
    fs.close();

    threads.reserve(core_num);

    map_files();
    multi_thread_reduce();
    split_pended_shard();
    multi_thread_reduce_pended_shard();
    multi_thread_merge_pended_shard();
    merge();
    return 0;
}
