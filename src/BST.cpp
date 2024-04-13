

#include "../inc/BST.hpp"


Node::Node(int value):
    data(value), left(nullptr), right(nullptr){} 

inline bool nodeLeft(int item, Node *node){
    return node->left == nullptr && node->data >= item;
}

inline bool nodeRight(int item, Node *node){
    return node->right == nullptr && node->data < item;
}

void BST::critical_write(int item){

    if(root == nullptr) root = new Node(item);
    else add(item, root);
}

void BST::add(int item, Node *node){

    if(nodeLeft(item, node)) node->left = new Node(item);
    else if(nodeRight(item, node)) node->right = new Node(item);
    else if(node->data <= item) add(item, node->right);
    else add(item, node->left);
}

std::vector<int> BST::critical_read(){
    std::vector<int> values;
    treeToString(&values, this->root);
    return values;
}

void BST::treeToString(std::vector<int> *values, Node *node){
    if(node == nullptr) return ;

    values->push_back( node->data);

    treeToString(values, node->left);
    treeToString(values, node->right);

}

BST::BST(){}