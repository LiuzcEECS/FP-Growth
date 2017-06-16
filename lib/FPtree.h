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
#include<utility>
#include "node.h"
#define MAX_LENGTH 500
using namespace std;

// the buffer for reading every lines of dataset
char _line[500];
// the frequent table ptr for compare function
map<string, int> * now_ft;
// total count of every frequent pattern
map<vector<string>, int> total_cnt;
// total count of every frequent pattern (the patterns' list has been sorted)
map<vector<string>, int> total_cnt_sort;
// total count of every frequent pattern (the patterns' list has been sorted and converted to a vector to sort by map's value)
vector<pair<vector<string>, int> > total_cnt_sort_vector;
// vector for nodes in a single path
vector<string> * input;
// stacks to find every combination of nodes in a single path (by dfs)
vector<string> * stack;

// compare function for vector converted from map (in order to sort by value)
bool cmp_map(pair<vector<string>, int> & a, pair<vector<string>, int> & b){
    return a.second > b.second;
}

// Class of whole FPtree and every step of FP-Growth
class FPtree{

public:

    /*
     * _sup: support
     * _bel: confidence
     * size: numbers of total transcations
     */
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
    void read(string _f){
        // read file stream
        string _item;
        ifstream _cin(_f.c_str());
        // hash_table for repetitive items
        set<string> _set;
        // vector used to store every transcation
        vector<string> _v;

        // reading csv file is a little different
        bool is_csv = false;
        if(_f.find("txt") == -1){
            is_csv = true;
            _cin.getline(_line, MAX_LENGTH);
        }
        while(_cin.getline(_line, MAX_LENGTH)){
            size++;
            _set.clear();
            _v.clear();
            istringstream _line_s(_line);
            //cout<<size<<" "<<_line<<endl;
            if(!is_csv){
                while(_line_s >> _item){
                    if(_set.find(_item) == _set.end()){
                        #ifdef _DEBUG
                        printf("%s ", _item.c_str());
                        #endif
                        _set.insert(_item);
                        _v.push_back(_item);
                    }
                }
            }
            // read csv file
            else{
                while(std::getline(_line_s, _item, ',')){
                    if(_set.find(_item) == _set.end()){
                        #ifdef _DEBUG
                        printf("%s ", _item.c_str());
                        #endif
                        _set.insert(_item);
                        _v.push_back(_item);
                    }
                }
            }
            //trans is lists of all transcations
            if(trans.find(_v) == trans.end()){
                trans[_v] = 1;
            }
            else{
                trans[_v] ++;
            }
        }

        //lim_s is support frequency, and lim_b is confidence
        lim_s = int(size * sup);
        lim_b = bel;
        printf("%d %d %.4f\n", size, lim_s, lim_b);
    }

    /**
     * construct: build a fp-tree
     * _root: the root of this tree
     * _trans: all the transcations in this sub-tree
     * _ft: frequent table of _trans
     * _item_list: every item in _trans
     * _header: the head ptrs of sibling linked lists
     */

    void construct(node *_root, map<vector<string>, int> & _trans, map<string, int> & _ft, vector<string> & _item_list, map<string, node *> & _header){
        // temp variables
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
            // at first we sort every transcation by the frequency of items
            sort(_vector.begin(), _vector.end(), cmp);

            for(int j = 0; j < _vector.size(); j++){
                // for every item, we build the tree from top to down
                
                // only ones more than support can be inserted into the tree
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
                        // the node already exists we only increase the frequency
                        _node = _node->child[_vector[j]];
                        _node->cnt += i->second;
                    }
                }
            }
        }

        //calculate item list and sort it
        for(map<string, int>::iterator i = _ft.begin(); i != _ft.end(); i++){
            //cout<<(i->first)<<" "<<(i->second)<<endl;
            if(i->second >= lim_s){
                _item_list.push_back(i->first);
            }
        }
        sort(_item_list.begin(), _item_list.end(), cmp);
        
        #ifdef _TREE
        // print the tree
        _root->vis(0);
        #endif

    }

    /**
     * cmp: function used to compare two items based on their frequency
     */
    static bool cmp(string & x, string & y){
        if((*now_ft)[x] == (*now_ft)[y]) return x > y;
        return (*now_ft)[x] > (*now_ft)[y];
    }

    
    /**
     * dfs: get every subset of the nodes on the single path
     * depth: depth of dfs
     */
    void dfs(int depth){
        if(depth == input->size()){
            // we get to the end of the path
            if(input->size() > 0){
                // save the combination
                if(total_cnt.find(*stack) == total_cnt.end()){
                    total_cnt[*stack] = (*now_ft)[(*stack)[stack->size() - 1]];
                }
            }
            //printf("hit the end\n");
        }
        else{
            // search for every combination
            dfs(depth + 1);
            stack->push_back((*input)[depth]);
            dfs(depth + 1);
            stack->pop_back();
        }
    }

    /**
     * fp_growth: the main recursive function for fp growth algorithm
     * item: name of the item which is being mining
     * item_head: the head this item's siblings in this conditional tree
     * last_ft: the frequent table of this conditional tree
     */
    void fp_growth(string & item, node * & item_head, map<string, int> & last_ft){
        // head of the siblings of this item
        node * _node = item_head;
        // ptr for leaf in a path
        node * leaf = NULL;
        // path on the tree
        vector<string> path;
        // the transcations in conditional pattern base
        map<vector<string>, int> next_trans;
        // loop through every sibling
        while(_node != NULL){
            // get paths of siblings and add it to transcation for next conditional tree
            leaf = _node->father;
            path.clear();
            while(leaf->father != NULL){
                path.push_back(leaf->name);
                leaf = leaf->father;
            }
            if(path.size() != 0){
                //in fp-tree the paths of different nodes will never be the same
                next_trans[path] = _node->cnt;
            }
            _node = _node->sibling;
        }

        // variables for next conditional tree
        node * next_root = new node("root", 1);
        map<string, int> next_ft;
        vector<string> next_item_list;
        map<string, node *> next_header;
        #ifdef _DEBUG
        printf("size of trans is %d \n", int(next_trans.size()));
        #endif

        // construct the conditional tree of this item
        construct(next_root, next_trans, next_ft, next_item_list, next_header);

        _node = next_root;
        // check whether there is only a single path in the next conditional tree
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
        #ifdef _DEBUG
        printf("isSingle is %d\n", is_single);
        #endif
        if(is_single){
            // if there is, just mining every combination of the prefix and save them

            prev.push_back(item);
            //cout<<prev[0]<<endl;
            total_cnt[prev] = last_ft[item];
            //if(next_item_list.size() > 0){
            input = &next_item_list;
            stack = &prev;
            dfs(0);
            prev.pop_back();
            //}
        }
        else{
            // if not, we push this item into prefix and save the new prefix as a fp
            prev.push_back(item);
            // then loop through every item of which frequency larger than support and try to get their conditional tree until the tree is empty or a single path
            for(int i = next_item_list.size() - 1; i >= 0; i--){
                total_cnt[prev] = last_ft[item];
                #ifdef _DEBUG
                printf("\nthis turn's item is %s\n", next_item_list[i].c_str());
                #endif
                fp_growth(next_item_list[i], next_header[next_item_list[i]], next_ft);
            }
            prev.pop_back();
        }
    }

    /**
     * cal: calculate the frequency pattern
     */
    void cal(){
        // first construct the tree
        construct(root, trans, ft, item_list, header);
        // then loop through every item of which frequency larger than support and try to get their conditional tree
        for(int i = item_list.size() - 1; i >= 0; i--){
            prev.clear();
            #ifdef _DEBUG
            printf("\nthis turn's item is %s\n", item_list[i].c_str());
            #endif
            fp_growth(item_list[i], header[item_list[i]], ft);
        }
    }


    /*
     * output: print every frequent pattern
     */
    void output(){
        printf("Frequent pattern\n");
        total_cnt_sort.clear();
        total_cnt_sort_vector.clear();
        now_ft = &ft;

        //loop through every fp and sort the items in a key in the order of frequency
        for(map<vector<string>, int>::iterator i = total_cnt.begin(); i != total_cnt.end(); i++){
            vector<string> _vector = i->first;
            int cnt = i->second;
            sort(_vector.begin(), _vector.end(), cmp);
            total_cnt_sort[_vector] = cnt;
            // convert map to vector
            total_cnt_sort_vector.push_back(pair<vector<string>, int>(_vector, cnt));
        }
        // sort the fp by the values
        sort(total_cnt_sort_vector.begin(), total_cnt_sort_vector.end(), cmp_map);

        for(int i = 0; i < total_cnt_sort.size(); i++){
            for(int j = 0; j < total_cnt_sort_vector[i].first.size(); j++){
                printf("%s ", (total_cnt_sort_vector[i].first)[j].c_str());
            }
            printf("%d \n", total_cnt_sort_vector[i].second);
        }
    }

    /**
     * aa: Associate analysis
     */
    void aa(){
        printf("Associate analysis\n");

        for(map<vector<string>, int>::iterator i = total_cnt_sort.begin(); i != total_cnt_sort.end(); i++){
            // temp variable
            vector<string> _vector = i->first;
            int cnt = _vector.size();

            // association rule looks like "A -> B"
            for(int l = 1; l < (1 << cnt) - 1; l++){
                // v1 is A and v2 is B
                vector<string> v1;
                vector<string> v2;
                // we represent the subset as a binary number
                for(int k = 1; k <= cnt; k++){
                    if((l >> (k - 1)) & 1) v1.push_back(_vector[k-1]);
                    else v2.push_back(_vector[k-1]);
                }
                // check if we get a wrong subset (I never meet an error)
                if(total_cnt_sort.find(v1) == total_cnt_sort.end()){
                    cout<<"error"<<endl;
                    continue;
                }
                // calculate the confidence of this rule
                double _bel = (i->second) / double(total_cnt_sort[v1]);
                // check is the confidence is larger than our limit
                if(_bel < lim_b) continue;
                for(int j = 0; j < v1.size(); j++){
                    printf("%s ", (v1)[j].c_str());
                }
                printf("-> ");
                for(int j = 0; j < v2.size(); j++){
                    printf("%s ", (v2)[j].c_str());
                }
                printf("%.4f\n", _bel);

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
    double lim_b;


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
    //list of prefix
    vector<string> prev;
};
