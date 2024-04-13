
#include "../inc/SharedResource.hpp"
#include "../inc/BST.hpp"

#define READERS 4
#define WRITERS 1
#define NUMBER_OF_READS 100
#define NUMBER_OF_WRITES 50

int main(){

    SharedResource<BST> rw(
        (ISharedResource*)(new BST()), 
        READERS, WRITERS, 
        NUMBER_OF_READS, 
        NUMBER_OF_WRITES
    );

    rw.start();

    
    
    return 0; 
}