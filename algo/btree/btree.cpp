/* Timothy Vicars
 * date: 11/9/19
 * CSCI 3352
 * BTree Implementation */
#include <iostream>
#include <vector>
#include <algorithm>
using std::vector;

typedef struct btnode {
    int* keys;  // array of keys
    int n;      // number of keys
    int t;      // minimum degree
    btnode** C; // child pointers
    bool leaf;  // node is leaf?

    btnode(int nt, bool nleaf) {
        t = nt;
        leaf = nleaf;
        n = 0;

        // reserve child and keys arrays.
        keys = new int[2 * t - 1];
        C = new btnode*[2 * t];
    }

    /* a sister function to insert() */
    void insert_nonfull(int k) {
        int i = n - 1;

        if (leaf) {
            // move everything to the right
            while (i >= 0 && keys[i] > k) {
                keys[i + 1] = keys[i];
                i--;
            }

            // and insert the key
            keys[i + 1] = k;
            n++;
        }
        else 
        {
            // find child
            while (i >= 0 && keys[i] > k) i--;

            // child full?
            if (C[i + 1]->n == 2 * t - 1) {
                splitChild(i + 1, C[i + 1]);

                if (keys[i + 1] < k)
                    i++;
            }
            C[i + 1]->insert_nonfull(k);
        }
    }

    /* used to split full children */
    void splitChild(int i, btnode* y) {
        btnode* z = new btnode(y->t, y->leaf);
        z->n = t - 1;
        /* copy keys */
        for (int j = 0; j < t - 1; j++) { 
            z->keys[j] = y->keys[j + t];
        }
        if (y->leaf == false) {
            for (int j = 0; j < t; j++) {
                z->C[j] = y->C[j + t];
            }
        }
        y->n = t - 1;

        /* copy children */
        for (int j = n; j >= i + 1; j--) 
            C[j + 1] = C[j];
        C[i + 1] = z;

        for (int j = n - 1; j >= i; j--) 
            keys[j + 1] = keys[j];

        keys[i] = y->keys[t - 1];
        n++;
    }

    void traverse() {
        int i;
        for (i = 0; i < n; i++) {
            if (!leaf)
                C[i]->traverse();
            std::cout << " " << keys[i];
        }

        /* traverse last child subtree */
        if (leaf == false)
            C[i]->traverse();
    }
} btnode;

typedef struct btree {
    btnode* root; 
    int t;       
    btree(int nt) {
        root = NULL;
        t = nt;
    }

    // function to traverse the tree
    void traverse() {
        if (root != NULL)
            root->traverse();
    }

    /* the main insertion function */
    void insert(int k) {
        /* tree empty? just make a new root */
        if (root == NULL) {
            root = new btnode(t, true);
            root->keys[0] = k; 
            root->n = 1;      
        }
        else
        {
            /* check if root is full
             * and split if so */
            if (root->n == 2 * t - 1) {
                btnode* s = new btnode(t, false);

                s->C[0] = root;
                s->splitChild(0, root);
                int i = 0;
                if (s->keys[0] < k)
                    i++;
                s->C[i]->insert_nonfull(k);

                root = s;
            }
            else 
                /* root is not full so insertnonfull */
                root->insert_nonfull(k);
        }
    }
} btree;

// main function to test implementation
int main() {
    /* create numbers to insert into tree */
    vector<int> mylist = {
        5,  7,  8, 20,
        4,  2, 66, 23,
        93, 9, 19, 24 };
    btree t(4);
    /* insert them into tree */
    for (int k : mylist)
        t.insert(k);

    std::cout << "traversal: ";
    t.traverse();
    std::cout << std::endl;
    return 0;
}
