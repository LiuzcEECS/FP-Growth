#include<iostream>
#include<fstream>
#include<sstream>
#include<hash_map>
#include<hash_set>
#include<set>
#include<map>
#include "node.hpp"
using namespace std;
char * _line[200];
class FPtree{
    FPtree(int _sup, int _bel = 0.7){
        sup = _sup;
        bel = _bel;
        trans.clear();
        result.clear();
        root = NULL;
    }
    /**
     * read: load the dataset
     * _f: dataset file path
     */
    void read(char * _f){
        // read file stream
        string _item;
        ifstream _cin(_f);
        // hash_table for repetitive items
        hash_set<string> _set;
        while(getline(_cin, _line)){
            _set.clear();
            _v.clear();
            istringstream _line_s(_line);
            while(_line_s >> _item){
                if(_set.find(_item) == _set.end()){
                    _set.insert(_item);
                    _v.push_back(item)
                }
            }
            if(trans.find(_v) == trans.end()){
                trans[_v] = 1;
            }
            else{
                trans[_v] ++;
            }
        }
    }

    /**
     * construct: build a fp-tree
     */
    void construct(){
        string tem;

        ft.clear();
        header.clear();
        rank.clear();

        //read every transcation and count the items
        for(map<vector<string>, int>::i = trans.begin(); i != trans.end(); i ++){
            for(int j = 0; j < (i->first).size()l j++){
                tem = (i->first)[j];
                if(ft.find(tem) == ft.end()){
                    ft[tem] = (i->second);
                }
                else{
                    ft[tem] += i->second;
                }
            }
        }
    }

    private:
        //support
        double sup;
        //belief
        double bel;
        //size of dataset
        int size;
        //min limit of support
        int lim_s;
        //min limit of belief
        int lim_b;


        //root of FPtree
        node * root;
        //table of transcation
        map<vector<string>, int> trans;
        //result
        map<vector<string>, int> result;
        //frequency table
        map<string, int> ft;
        //header of the frequency table
        map<string, node *> header;
        //rank
        vector<string> rank;
}
