#ifndef TOP_100_URL_ZSET_H
#define TOP_100_URL_ZSET_H

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;
struct node_t{
    unsigned long count;
    unsigned long index;
    node_t(unsigned long c,unsigned long i):count(c),index(i){}
};
class zset{
private:
    typedef unordered_map<string,node_t> zmap_t;
public:
    zset();
    bool add(const string& key);
    bool inc(const string& key);
    void create_or_inc(const string &key,int count=1);
    //return top url and its count
    vector<pair<string,int>> pop(int num=1);
    zmap_t::size_type size(){ return m_map.size();}
    void printdebug();
private:
    //string key===>count , index in m_vec
    zmap_t m_map;
    vector<zmap_t::iterator> m_vec;
    static bool fcmp_less(const zmap_t::iterator &it1, const zmap_t::iterator &it2);
    static bool fcmp_less_equal(const zmap_t::iterator &it1, const zmap_t::iterator &it2);
    void heapify(const zmap_t::iterator &it_in);
};

#endif //TOP_100_URL_ZSET_H
