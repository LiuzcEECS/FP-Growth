#include<iostream>
#include<string>
#include<math.h>
#include<stdlib.h>
#include "../lib/FPtree.hpp"

using namespace std;

int main(int argc, char ** argv){

    int i;
    string path = char[0];
    double sup = atof(char[1]);
    double bel = atof(char[2]);

    FPtree f(sup, bel);
    f.read(path);
    f.construct();
    //f.cal();
    //f.output();
    delete f;
    return 0;
}
