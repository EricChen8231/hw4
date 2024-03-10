#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    if (root == nullptr){
        return true;
    }
    int lheight = height(root->left);
    int rheight = height(root->right);
    if (lheight != rheight){
        return false;
    }
    return equalPaths(root->left) && equalPaths(root->right);
}

int height(Node* root){
    if (root == nullptr){
        return 0;
    }
    int lheight = height(root->left);
    int rheight = height(root->right);
    return max(lheight, rheight) + 1;
}

