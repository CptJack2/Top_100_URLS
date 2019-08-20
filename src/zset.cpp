#include "zset.h"

zset::zset() {

}
bool zset::add(const string& key) {
    if(m_map.find(key)!=m_map.end())
        return false;
    zmap_t::iterator it=m_map.insert(make_pair(key,node_t(1,0))).first;
    m_vec.push_back(it);
    it->second.index=m_vec.size()-1;
    push_heap(m_vec.begin(),m_vec.end(),zset::fcmp_less);
    return true;
}
bool zset::inc(const string& key) {
    auto it=m_map.find(key);
    if(it==m_map.end())
        return false;
    ++(it->second.count);
    //find the node's father, and adjust the vector to fullfill the heap's requirement
    heapify(it);
    return true;
}
bool zset::fcmp_less(const zmap_t::iterator &it1, const zmap_t::iterator &it2){
    return it1->second.count < it2->second.count;
}
bool zset::fcmp_less_equal(const zmap_t::iterator &it1, const zmap_t::iterator &it2){
    return it1->second.count <= it2->second.count;
}
void zset::heapify(const zmap_t::iterator& it_in) {
    if(it_in->second.index==0)
        return;
    unsigned long index=(it_in->second.index-1)/2;
    zmap_t::iterator it=m_vec[index],
        lit=index*2+1<m_vec.size()?m_vec[index*2+1]:m_map.end(),
        rit=(index+1)*2<m_vec.size()?m_vec[(index+1)*2]:m_map.end(),
        largest;
    if(lit!=m_map.end() && fcmp_less(it,lit))
        largest=lit;
    else
        largest=it;
    if(rit!=m_map.end() && fcmp_less(it,rit))
        largest=rit;
    if(largest!=it){
        //swap iter
        m_vec[largest->second.index]=it;
        m_vec[it->second.index]=largest;
        //swap index
        unsigned long t=largest->second.index;
        largest->second.index=it->second.index;
        it->second.index=t;
        //recursively heapify
        heapify(largest);
    }
}
void zset::printdebug(){
    for(int i=0;i<m_vec.size();i++)
        cout<<m_vec[i]->first<<" ";
    cout<<endl;
    for(int i=0;i<m_vec.size();i++)
        cout<<m_vec[i]->second.index<<" ";
    cout<<endl;
    for(int i=0;i<m_vec.size();i++)
        cout<<m_vec[i]->second.count<<" ";
    cout<<endl<<endl;
}
vector<pair<string,int>> zset::pop(int num){
    vector<pair<string,int>> ret;
    int size=min<int>(num,m_vec.size());
    for(int i=1;i<=size;++i){
        //wtf?! using the same comparator as push will cause the elements with same value being swaped, shouldn't suppose to be like that
        pop_heap(m_vec.begin(),m_vec.end(),zset::fcmp_less_equal);
        ret.emplace_back(m_vec.back()->first,m_vec.back()->second.count);
        m_vec.pop_back();
    }
    for(int i=0;i<m_vec.size();++i)
        m_vec[i]->second.index=i;
    return ret;
}
void zset::create_or_inc(const string &key,int count){
    auto it=m_map.find(key);
    if(it==m_map.end()){
        zmap_t::iterator it=m_map.insert(make_pair(key,node_t(1,0))).first;
        m_vec.push_back(it);
        it->second.index=m_vec.size()-1;
        push_heap(m_vec.begin(),m_vec.end(),zset::fcmp_less);
    }
    else{
        it->second.count+=count;
        //find the node's father, and adjust the vector to fullfill the heap's requirement
        heapify(it);
    }
}