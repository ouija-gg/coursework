#include <vector>
#include <algorithm>
#include <iostream>
using std::vector;

typedef struct btnode {
    //int* keys; // array of all this node's keys
    //int n; //keys used
    vector<int> keys;
    int t; // min degree
    //struct btnode* child; // an array of children
    vector<struct btnode*> child;
    bool leaf; // leaf?

    // constructor
    btnode(int nt, bool nleaf) {
        t = nt;
        leaf = nleaf;
    };

    void split_child(btnode* y, int i) {
        std::cout << "splitting " << y->keys[i] << std::endl;
        btnode* z = new btnode(y->t, y->leaf);
        // copy (t-1) keys to z
        for (int j = t+1; j < 2 * t; j++) {
            z->keys.push_back(y->keys[j]);
        }
        std::cout << "copied all keys\n";
        this->keys.resize(t);
        std::cout << "resized keys\n";

        // copy children to z
        if (!y->leaf) {
            for (int j = t+1; j <= 2*t; i++)
                z->child.push_back(this->child[i]);
            this->child.resize(t+1);
            /*
            for (int j = 0; j < t; j++) {
                z->child.push_back(y->child[j+t]);
                y->child.erase(y->child.begin()+j+t);
            }
            */
        }
        std::cout << "copied children\n";

        child.insert(child.begin()+i, z);
        std::cout << "inserted (t)child from z\n";
        
        keys.insert(keys.begin() + i,y->keys[t-1]);
        std::cout << "inserted key from z\n";
    }

    /* this is a "simplified" function to insert
     * into non-full nodes */
    void insert_nonfull(int k) {
        vector<int>::iterator it;
        std::cout << "inserting nonfull " << k << std::endl;
        //vector<int>::reverse_iterator it;

        if (leaf) {
            std::cout << "leaf inserting " << k << std::endl;
            it = std::lower_bound(keys.begin(), keys.end(), k);
            keys.insert(it, k);
            std::cout << "inserted " << k << std::endl;
        }
        else {
            int i = keys.size() - 1;
            std::cout << "nonleaf i: " << i << std::endl;
            // find the child we can insert k into
            while (i >= 0 && keys[i] > k)
                i--;

            std::cout << "child keysize: " << child[i+1]->keys.size() << std::endl;
            if (child[i+1]->keys.size() == 2*t-1) {
                split_child(child[i+1], i+1);
                if (keys[i+1] < k)
                    i++;
            }
            child[i+1]->insert_nonfull(k);
            
            /*int i = keys.size();
            for (; i > 0 && k < keys[i]; i--);
            i++;
            // potential child to insert into is at child[i]
            if (child[i]->keys.size() == 2*t - 1) {
                // if the child is full, split it
                split_child(child[i], i);

                if (keys[i] < k)
                    i++;
            }
            child[i]->insert_nonfull(k);
            */
            std::cout << "nonleaf inserted " << k << std::endl;
           
        }
    }

    void traverse() {
        if (!leaf) 
            for (auto nchild : child)
                nchild->traverse();
        for ( auto key : keys )
            std::cout << key << "  ";


    }
} btnode;

typedef struct btree {
    btnode* root;
    int t;

    // constructor
    btree(int nx) {
        root = new btnode(nx, true);
        t = nx;
    };

    void traverse() {
        if (root != NULL) {
            std::cout << "List: ";
            root->traverse();
            std::cout << std::endl;
        }
    }
    // insert key into the tree
    void insert(int k) {
        std::cout << "inserting " << k << std::endl;
        // from book
        btnode* r = root;

        // is root full?
        std::cout << "keysize " << r->keys.size() << std::endl;
        if (root->keys.size() == 2*t - 1) {
            std::cout << " root is full" << std::endl;
            btnode* s = new btnode(t, false);
            std::cout << " made s" << std::endl;
            s->child.push_back(root);
            s->split_child(root,0);

            int i = 0;
            if (s->keys[0] < k) i=1;
            s->child[i]->insert_nonfull(k);
            root = s;
            std::cout << "made root = s" << std::endl;


            /*// create a new child and to split root with
            btnode* s = new btnode(t, false);
            root = s;
            s->child.push_back(r);
            s->split_child(root, 0);
            root->insert_nonfull(k);
            */
        }
        else {
            root->insert_nonfull(k);
        }
        traverse();
    }
} btree;
