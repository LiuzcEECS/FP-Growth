using namespace std;

//Class of nodes of FP tree
class node{
public:

    /*
     * _name: name of this item
     * _cnt: the frequency of this node
     * _father: the parent of this node
     */

    node(string _name, int _cnt, node * _father = NULL){
        name = _name;
        cnt = _cnt;
        father = _father;
        sibling = NULL;
        child.clear();
    }

    /*
     * vis: print the tree
     * depth: the depth of this node
     */

    void vis(int depth){
        //print the branch of this node
        for(int i = 1; i <= depth; i++){
            printf("----------");
        }
        // print the name
        printf("%s: %d\n", name.c_str(), cnt);
        //print its children
        for(map<string, node *>::iterator i = child.begin(); i!=child.end(); i++){
            printf("|");
            (i->second)->vis(depth+1);
        }
    }

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

};
