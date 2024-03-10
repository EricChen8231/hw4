#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkleaf(Node *root, int height);
int height(Node*root);
bool equalPaths(Node * root)
{
    if (root == nullptr){
        return true;
    }
    int height1 = height(root)-1;
    if (root->left == nullptr){
        return checkleaf(root->right, height1);
    }
    if (root->right == nullptr){
        return checkleaf(root->left, height1);
    }
    return checkleaf(root->left, height1) && checkleaf(root->right, height1);
}

int height(Node *root)
{
    if (root == nullptr)
    {
        return 0; 
    }
    int lheight = height(root->left);
    int rheight = height(root->right);
    return std::max(lheight, rheight) + 1;
}
bool checkleaf(Node *root, int height)
{
    if (root == nullptr)
    {
        if (height == 0){
            return true;
        }
        return false;
    }
    // if its a leaf node and the height doesnt match return false
    if (root->left == nullptr && root->right == nullptr)
    {
        if (height == 1){
            return true;
        }
        return false; 
    }
    // if its has only 1 child
    if (root->left == nullptr){
        return checkleaf(root->right, height-1);
    }
    if (root->right == nullptr){
        return checkleaf(root->left, height-1);
    }
    return checkleaf(root->left, height-1) && checkleaf(root->right, height-1);
}

