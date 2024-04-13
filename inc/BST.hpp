#ifndef BST_HPP_
#define BST_HPP_ 

// c++ lib 
#include "SharedResource.hpp"
#include <vector>

// node with unique_ptr that uses RAII that will handle the clean up of resources
struct Node
{
    int data;
    Node *left;
    Node *right; 
    Node(int item);
};


// class BST will include 
// mutexes and semaphores 
// to handle concurrent read
// write operations
class BST : ISharedResource
{
private:
    void add(int item, Node *node);
    void treeToString(std::vector<int> *values, Node *node);
public:
    Node *root;
    void critical_write(int item) override;
    std::vector<int> critical_read() override;
    BST();

};

#endif 