#include<iostream>
using namespace std;
class node{

    node(string _name, int _cnt, node * _father = NULL){
        name = _name;
        cnt = _cnt;
        father = _father;
        sibling = null;
        child.clear();
    }

    void vis(int depth){
        for(int i = 1; i <= k; i++){
            printf("-------");
        }
        if(father == NULL){
            printf("R");
        }
        else{
            printf("%s: %d\n", name.c_str(), cnt);
        }
        for(map<string, node *>::iterator i; i!=child.end(); i++){
            printf("|");
            (i->second)->vis(depth+1);
        }
    }

    private:
        // count of the item
        int cnt;
        // name of the item
        string name;
        // list of children
        map<string, node *> child;
        // father of this node
        node * father;
        // sibling of this node
        node * sibling;

}
