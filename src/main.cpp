#include "zset.h"
#include <iostream>
#include <fstream>
using namespace std;

int Sharding_Num=120;
int TopK=100;
string filen="Dataset.txt";

int main(){
    zset zset1;
    for(int i=0;i<=9;i++)
        zset1.create_or_inc("cxl" + to_string(i));
    zset1.printdebug();
    int a=1;
    zset1.pop();
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
