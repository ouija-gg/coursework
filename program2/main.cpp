#include "btree.h"
#include <iostream>

int main() {
    btree t(4);
    t.insert(20);
    std::cout << "size: " << t.root->keys.size() << std::endl;
    t.insert(30);
    t.insert(2);
    t.insert(9);
    t.insert(880);
    t.insert(37);
    t.insert(91);
    t.insert(55);
    t.insert(13);
    t.insert(22);
    t.insert(34);
    t.insert(44);
    t.insert(6);
    t.insert(40);
    t.insert(60);
    t.insert(33);

    std::cout << "my list: ";
    t.traverse();
    
    return 0;
}
