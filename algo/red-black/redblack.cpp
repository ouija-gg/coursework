struct node {
    const int data;
    node* parent;
    node* right;
    node* left;
    enum color { black, red } color;

    // constructor
    node(int d)
    : data(d), parent(nullptr), right(nullptr), left(nullptr), color(red){};
};
struct rb_tree {
    node* root;
};

void rb_transplant(node* z, node* y);

void fixup() {}

void rb_delete() {}

void rb_insert() {}

node* tree_min(node* z) {
    while (z->left != nullptr) z = z->left;
    return z;
}

node* tree_max(node* z) {
    while (z->right != nullptr) z = z->right;
    return z;
}

int main() {
    node hi = node(42);

    return 0;
}
