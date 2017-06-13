#include<iostream>
#include<fstream>
#include<sstream>
#include<set>
#include<map>
#include<vector>
#include<algorithm>
#include<string>
#include<memory.h>
#include<stdio.h>
#include<cstdio>
#include "node.h"
#define MAX_LENGTH 200
using namespace std;

char _line[200];
class FPtree{

    public:

    FPtree(double _sup, double _bel){
        sup = _sup;
        bel = _bel;
        size = 0;
        trans.clear();
        result.clear();
        root = NULL;
    }

    ~FPtree(){
    }

    /**
     * read: load the dataset
     * _f: dataset file path
     */
    void read(const char * _f){
        // read file stream
        string _item;
        ifstream _cin(_f);
        // hash_table for repetitive items
        set<string> _set;
        vector<string> _v;
        while(_cin.getline(_line, MAX_LENGTH)){
            size++;
            _set.clear();
            _v.clear();
            istringstream _line_s(_line);
            while(_line_s >> _item){
                if(_set.find(_item) == _set.end()){
                    //printf("%s ", _item.c_str());
                    _set.insert(_item);
                    _v.push_back(_item);
                }
            }
            if(trans.find(_v) == trans.end()){
                trans[_v] = 1;
            }
            else{
                trans[_v] ++;
            }
        }

        lim_s = int(size * sup);
        lim_b = int(size * bel);
        //printf("%d %d %d\n", size, lim_s, lim_b);
    }

    /**
     * construct: build a fp-tree
     */
    void construct(){
        string _string;
        vector<string> _vector;
        node * _node = NULL;

        ft.clear();
        item_list.clear();
        header.clear();

        //read every transcation and count the items
        for(map<vector<string>, int>::iterator i = trans.begin(); i != trans.end(); i++){
            for(int j = 0; j < (i->first).size(); j++){
                _string = (i->first)[j];
                if(ft.find(_string) == ft.end()){
                    ft[_string] = (i->second);
                }
                else{
                    ft[_string] += i->second;
                }
            }
        }

        for(map<string, int>::iterator i = ft.begin(); i != ft.end(); i++){
            if(i->second >= lim_s){
                item_list.push_back(i->first);
            }
        }
        sort(item_list.begin(), item_list.end(), cmp);

        //build the original fp-tree
        root = new node("root", 1);
        for(map<vector<string>, int>::iterator i = trans.begin(); i != trans.end(); i++){
            _node = root;
            _vector = i->first;
            sort(_vector.begin(), _vector.end(), cmp);
            for(int j = 0; j < _vector.size(); j++){
                if(ft[_vector[j]] >= lim_s){
                    if(_node->child.find(_vector[j]) == _node->child.end()){
                        //insert a node
                        _node->child[_vector[j]] = new node(_vector[j], i->second, _node);
                        _node = _node->child[_vector[j]];
                        //insert a sibling
                        if(header.find(_vector[j]) == header.end()){
                            header[_vector[j]] = _node;
                        }
                        else{
                            _node->sibling = header[_vector[j]];
                            header[_vector[j]] = _node;
                        }
                    }
                    else{
                        _node = _node->child[_vector[j]];
                        _node->cnt += i->second;
                    }
                }
            }
        }

        root->vis(0);

    }

    /**
     * cmp: function used to compare two items based on their frequency
     */
    static bool cmp(string & x, string & y){
        return ft[x] > ft[y];
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
        static map<string, int> ft;
        //list of frequent 1-itemsets (sets of only one item)
        vector<string> item_list;
        //head of every item in the fp-tree
        map<string, node *> header;
};
    map<string, int> FPtree::ft;
