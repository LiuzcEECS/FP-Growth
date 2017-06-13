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
map<string, int> * now_ft;
map<vector<string>, int> total_cnt;
vector<string> * input;
vector<string> * stack;

class FPtree{

public:

    FPtree(double _sup, double _bel){
        sup = _sup;
        bel = _bel;
        size = 0;
        trans.clear();
        result.clear();
        root = new node("root", 1);
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
    void construct(node *_root, map<vector<string>, int> & _trans, map<string, int> & _ft, vector<string> _item_list, map<string, node *> & _header){
        string _string;
        vector<string> _vector;
        node * _node = NULL;

        _ft.clear();
        _item_list.clear();
        _header.clear();
        now_ft = &_ft;

        //read every transcation and count the items
        for(map<vector<string>, int>::iterator i = _trans.begin(); i != _trans.end(); i++){
            for(int j = 0; j < (i->first).size(); j++){
                _string = (i->first)[j];
                if(_ft.find(_string) == _ft.end()){
                    _ft[_string] = (i->second);
                }
                else{
                    _ft[_string] += i->second;
                }
            }
        }

        //build the original fp-tree
        for(map<vector<string>, int>::iterator i = _trans.begin(); i != _trans.end(); i++){
            _node = _root;
            _vector = i->first;
            sort(_vector.begin(), _vector.end(), cmp);
            for(int j = 0; j < _vector.size(); j++){
                if(_ft[_vector[j]] >= lim_s){
                    if(_node->child.find(_vector[j]) == _node->child.end()){
                        //insert a node
                        _node->child[_vector[j]] = new node(_vector[j], i->second, _node);
                        _node = _node->child[_vector[j]];
                        //insert a sibling
                        if(_header.find(_vector[j]) == _header.end()){
                            _header[_vector[j]] = _node;
                        }
                        else{
                            _node->sibling = _header[_vector[j]];
                            _header[_vector[j]] = _node;
                        }
                    }
                    else{
                        _node = _node->child[_vector[j]];
                        _node->cnt += i->second;
                    }
                }
            }
        }

        for(map<string, int>::iterator i = _ft.begin(); i != _ft.end(); i++){
            if(i->second >= lim_s){
                _item_list.push_back(i->first);
            }
        }
        sort(_item_list.begin(), _item_list.end(), cmp);

        _root->vis(0);

    }

    /**
     * cmp: function used to compare two items based on their frequency
     */
    static bool cmp(string & x, string & y){
        return (*now_ft)[x] > (*now_ft)[y];
    }

    /**
     * cal: calculate the frequency pattern
     */
    void cal(){
        construct(root, trans, ft, item_list, header);
        for(int i = item_list.size() - 1; i >= 0; i++){
            prev.clear();
            fp_growth(item_list[i]);
        }
    }

    /**
     * get_subset: get every subset of the input set
     * input: input set
     * result: the list of output results
     */
    void dfs(int depth){
        if(depth == input->size()){
            total_cnt[*stack] = (*now_ft)[(*input)[0]];
        }
        else{
            dfs(depth + 1);
            stack->push_back((*input)[depth]);
            dfs(depth + 1);
            stack->pop_back();
        }
    }

    /**
     * fp_growth: 
     * item: name of the item which is being mining
     * prev: the items which are already part of the frequent pattern
     */
    void fp_growth(string & item){
        node * _node = header[item];
        node * leaf = NULL;
        vector<string> path;
        map<vector<string>, int> next_trans;
        while(_node != NULL){
            leaf = _node;
            path.clear();
            _node = _node->sibling;
            while(leaf->father != NULL){
                leaf = leaf->father;
                path.push_back(leaf->name);
            }
        }
        if(_node->father != NULL){
            //in fp-tree the path will never be same as others.
            next_trans[path] = _node->cnt;
        }

        node * next_root = new node("R", 1);
        map<string, int> next_ft;
        vector<string> next_item_list;
        map<string, node *> next_header;
        construct(next_root, next_trans, next_ft, next_item_list, next_header);

        _node = next_root;
        bool is_single = true;
        while(1){
            if(_node->child.size() > 1){
                is_single = false;
                break;
            }
            else if(_node->child.size() <= 0){
                break;
            }
            _node = _node->child.begin()->second;
        }
        if(is_single){
            prev.push_back(item);
            if(next_item_list.size() > 0){
                input = &next_item_list;
                stack = &prev;
                dfs(0);
                delete stack;
            }
        }
        else{
            prev.push_back(item);
            for(int i = next_item_list.size() - 1; i >= 0; i--){
                fp_growth(next_item_list[i]);
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
    //list of frequent 1-itemsets (sets of only one item)
    vector<string> item_list;
    //head of every item in the fp-tree
    map<string, node *> header;
    //list of suffix
    vector<string> prev;
};
