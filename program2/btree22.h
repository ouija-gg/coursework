#include <vector>
#include <iterator>
#include <iostream>
using std::vector;
#define Degree 4

typedef struct btnode {
    //int* keys; // array of all this node's keys
    //int n; //keys used
    vector<int>* keys;
    int t; // min degree
    //struct btnode* child; // an array of children
    vector<struct btnode*>* child;
    bool leaf; // leaf?

    // constructor
    btnode(int nt, bool nleaf) {
        t = nt;
        leaf = nleaf;

        keys->reserve(2*t-1);
        child->reserve(2*t);
    };

    void split_child(btnode* y, int i) {
        btnode* z = new btnode(y->t, y->leaf);
        // copy keys to z
        for (int j = 0; j < t-1; j++) {
            z->keys->push_back(*(y->keys[j+t]));
            y->keys->erase(j+t);
        }
        if (!y->leaf) {
            // copy children to z
            for (int j = 0; j < t; j++) {
                z->child->push_back(y->child[j+t]);
                y->child->erase(j+t);
            }
        }
        child->insert(i+1, z);
        
        keys->insert(i,y->keys[t-1]);
    }

    void traverse() {
        for (int i = child->size(); i++) {
            traverse(child[i]);
            std::cout << " " << keys[i];
        }
        child[child->size()-1]->traverse();
    }
        


} btnode;

typedef struct btree {
    btnode* root;
    int t;

    // constructor
    btree(int nx) {
        root = NULL;
        t = nx;
    };

    // insert key into the tree
    void insert(int k) {
        // from book
        btnode* r = root;

        // is root full?
        if (r->keys->size == 2t - 1) {
            // create a new child and to split root with
            btnode* s = new btnode(t, false);
            root = s;
            s->child->push_back(r);
            split_child(s, 0);
            root->insert_nonfull(k);
        }
        else {
            root->insert_nonfull(k);
        }
    }

    /* this is a "simplified" function to insert
     * into non-full nodes */
    void insert_nonfull(int k) {
        vector<int>::reverse_iterator it;
        if (leaf) {
            for (it = keys->rbegin(); it != keys->rend(); it++) {
                if (k > *it) {
                    keys->insert(it.base(), k);
                    break;
                }
            }
        }
        else {
            /* find the child we can insert k into */
            int i = keys.size();
            for (; i > 0; i--) {
                if (k > key[i])
                    break;
            }
            i++;
            // potential child to insert into is at child[i]
            if (child[i].keys.size() == 2*t - 1) {
                // if the child is full, split it
                split_child(child[i], i);

                if (keys[i] < k)
                    i++;
            }
            child[i]->insert_nonfull(k);
        }
    }
} btree;
