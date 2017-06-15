#include<iostream>
#include<string>
#include<math.h>
#include<stdlib.h>
//#define _DEBUG
//#define _TREE
#include "../lib/FPtree.h"

using namespace std;

int main(int argc, char ** argv){

    int i;
    //data file path
    string path = argv[1];
    //support
    double sup = atof(argv[2]);
    //confidence
    double bel = 0.5;
    if(argc > 3){
        bel = atof(argv[3]);
    }
    FPtree * f = new FPtree(sup, bel);
    //read the data
    f->read(path.c_str());
    //Mining frequent patterns
    f->cal();
    //Output frequent patterns
    f->output();
    //Association analysis
    printf("\n");
    f->aa();
    delete f;
    return 0;
}
