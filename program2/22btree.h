#include <vector>
#include <iterator>
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

        keys.reserve(2*t-1);
        child.reserve(2*t);
    };

    void split_child(btnode* y, int i) {
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
        std::cout << "inserting nonfull " << k << std::endl;
            std::cout << "leaf " << leaf << std::endl;
        vector<int>::reverse_iterator it;
            std::cout << "leaf " << leaf << std::endl;
        if (leaf) {
            vector<int>::iterator it;
            it = std::lower_bound(keys.begin(), keys.end(), k);
            keys.insert(it, k);
            std::cout << "inserted " << k << std::endl;
        }
        else {
            /* find the child we can insert k into */
            int i = keys.size();
            for (; i > 0; i--) {
                if (k > keys[i])
                    break;
            }
            i++;
            // potential child to insert into is at child[i]
            if (child[i]->keys.size() == 2*t - 1) {
                // if the child is full, split it
                split_child(child[i], i);

                if (keys[i] < k)
                    i++;
            }
            child[i]->insert_nonfull(k);
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
        if (r->keys.size() == 2*t - 1) {
            std::cout << " root is full" << std::endl;
            btnode* s = new btnode(t, false);
            s->child[0] = root;
            std::cout << "splitting : root" << std::endl;
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
