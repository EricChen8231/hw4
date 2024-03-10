#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkleaf(Node *root);

bool equalPaths(Node * root)
{
    if (root == nullptr){
        return true;
    }
    
    return checkleaf(root->left) && checkleaf(root->right);
}
bool checkleaf(Node *root)
{
    if (root == nullptr)
    {
        return true;
    }

    if (root->left == nullptr && root->right == nullptr)
    {
        return true; 
    }
    return checkLeafDistances(root->left) && checkLeafDistances(root->right);
}

