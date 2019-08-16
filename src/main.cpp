#include "zset.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <thread>
#include <mutex>

using namespace std;

const int core_num=4;
const unsigned long Sharding_Num=100;
const int TopK=100;
const string file_name="../data/Data.txt";

unsigned long file_len=0;
vector<thread> threads;

vector<ofstream> map_ofss(Sharding_Num);
vector<mutex> map_ofss_mutexs(Sharding_Num);
void map_files(int thread_index) {
    ifstream ifs(file_name);
    const unsigned long pos = file_len * thread_index / Sharding_Num;
    ifs.seekg(pos);
    string str;
    //if not at file head, abandon the first line( let previous thread process it)
    if (thread_index != 0)
        ifs >> str;
    hash<string> hasher;
    while (ifs.tellg() <= pos + file_len / Sharding_Num) {
        str.clear();
        ifs >> str;
        if (str.empty())continue;
        int index = hasher(str) % Sharding_Num;
        lock_guard<mutex> lg(map_ofss_mutexs[index]);
        map_ofss[index] << str << endl;
    }

    ifs.close();
}
void multi_thread_map() {
    threads.clear();
    //open output streams
    for (int i = 0; i < Sharding_Num; ++i)
        map_ofss[i].open("../data/shard_" + to_string(i) + ".txt", ios::out);
    for (int i = 0; i < core_num; ++i)
        threads.emplace_back(map_files, i);

    //wait for the mapping threads to end and close the resources
    for (auto &t:threads)t.join();
    for (auto &ofs:map_ofss)ofs.close();
    threads.clear();
}
void reduce_files(int thread_index){
    for(int i=thread_index;i<Sharding_Num;i+=core_num){
        ifstream ifs;
        ifs.open("../data/shard_"+to_string(i)+".txt",ios::in);
        zset myzset;
        string str;

        while(!ifs.eof()){
            //stupid fucking fstream remain the string the way it was in last cycle when it read a blank line, fuck
            str.clear();
            ifs>>str;
            if(str.empty())continue;
            myzset.create_or_inc(str);
        }
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
void test_thread(int i){
    cout<<"thread: "<<i<<endl;
}
int main(){
    //get file length
    fstream fs(file_name);
    fs.seekg(0, ios_base::end);
    file_len = fs.tellg();
    fs.close();

    threads.reserve(core_num);

    multi_thread_map();
    multi_thread_reduce();
    merge();
    return 0;
}
