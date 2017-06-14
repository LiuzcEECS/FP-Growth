#include<iostream>
#include<string>
#include<math.h>
#include<stdlib.h>
//#define _DEBUG
//#define _TREE
//#define _PARA
#include "../lib/FPtree.h"

using namespace std;

int main(int argc, char ** argv){

    int i;
    string path = argv[1];
    double sup = atof(argv[2]);
    double bel = 0.7;
    if(argc > 3){
        double bel = atof(argv[3]);
    }
    FPtree * f = new FPtree(sup, bel);
    f->read(path.c_str());
    f->cal();
    f->output();
    delete f;
    return 0;
}
