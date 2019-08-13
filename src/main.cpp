#include "zset.h"
#include <iostream>
#include <fstream>
using namespace std;

int Sharding_Num=120;
int TopK=100;
string filen="Dataset.txt";

int main(){
    zset zset1;
    fstream fs("/home/cptjack/Dataset.txt");
    string str;
    fs.seekg(0, ios_base::end);
    unsigned long nFileLen = fs.tellg();
    int pos=nFileLen/10;
    fs.seekg(pos);
    while(fs.peek()!='\n')
        fs.seekg(++pos);
    fs.seekg(++pos);
    fs>>str;
    for(int i=0;i<100;++i) {
        fs >> str;
        zset1.create_or_inc(str);
    }
    zset1.printdebug();
    int a=1;
    zset1.pop(3);
    zset1.printdebug();

    zset1.pop();
    zset1.printdebug();
    zset1.pop();
    zset1.printdebug();

    zset1.inc("cxl6");
    zset1.printdebug();
    zset1.inc("cxl7");
    zset1.printdebug();
    zset1.inc("cxl7");
    zset1.printdebug();
    zset1.inc("cxl3");
    zset1.printdebug();
    zset1.inc("cxl3");
    zset1.printdebug();
    zset1.inc("cxl3");
    zset1.printdebug();
    int c=1;
    zset1.pop();
    zset1.printdebug();
    zset1.pop();
    zset1.printdebug();
    zset1.pop();
    zset1.printdebug();
    int b=1;
}
