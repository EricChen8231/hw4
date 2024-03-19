#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError
{
};

/**
 * A special kind of node for an AVL tree, which adds the balance as a data member, plus
 * other additional helper functions. You do NOT need to implement any functionality or
 * add additional data members or helper functions.
 */
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key &key, const Value &value, AVLNode<Key, Value> *parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance() const;
    void setBalance(int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value> *getParent() const override;
    virtual AVLNode<Key, Value> *getLeft() const override;
    virtual AVLNode<Key, Value> *getRight() const override;

protected:
    int8_t balance_; // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
 * An explicit constructor to initialize the elements by calling the base class constructor
 */
template <class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key &key, const Value &value, AVLNode<Key, Value> *parent) : Node<Key, Value>(key, value, parent), balance_(0)
{
}

/**
 * A destructor which does nothing.
 */
template <class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{
}

/**
 * A getter for the balance of a AVLNode.
 */
template <class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
 * A setter for the balance of a AVLNode.
 */
template <class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
 * Adds diff to the balance of a AVLNode.
 */
template <class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
 * An overridden function for getting the parent since a static_cast is necessary to make sure
 * that our node is a AVLNode.
 */
template <class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value> *>(this->parent_);
}

/**
 * Overridden for the same reasons as above.
 */
template <class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value> *>(this->left_);
}

/**
 * Overridden for the same reasons as above.
 */
template <class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value> *>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert(const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key &key);                              // TODO
    void remove(AVLNode<Key, Value> *root, const Key &key);

protected:
    virtual void nodeSwap(AVLNode<Key, Value> *n1, AVLNode<Key, Value> *n2);

    void leftRotate(AVLNode<Key, Value> *node);
    void rightRotate(AVLNode<Key, Value> *node);
    void rebalance(AVLNode<Key, Value> * node);
    void rebalancehelper(AVLNode<Key, Value> * node);
    // Add helper functions here
};

template <class Key, class Value>
void AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value> *node)
{
    if (node == nullptr || node->getRight() == nullptr)
    {
        return;
    }
    AVLNode<Key, Value> *nright = node->getRight();
    AVLNode<Key, Value> *nparent = node->getParent();
    AVLNode<Key, Value> *nrightleft = nright->getLeft();
    nright->setParent(nparent);
    if (nparent != nullptr)
    {
        if (nparent->getRight() == node)
        {
            nparent->setRight(nright);
        }
        else
        {
            nparent->setLeft(nright);
        }
    }
    else
    {
        this->root_ = nright;
    }

    node->setParent(nright);
    node->setRight(nrightleft);
    if (nrightleft != nullptr)
    {
        nrightleft->setParent(node);
    }
    nright->setLeft(node);

    // Update balance factors
    node->setBalance(this->height(node->getLeft()) - this->height(node->getRight()));
    nright->setBalance(this->height(nright->getLeft()) - this->height(nright->getRight()));
}

template <class Key, class Value>
void AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value> *node)
{
    if (node == nullptr || node->getLeft() == nullptr)
    {
        return;
    }
    AVLNode<Key, Value> *nleft = node->getLeft();
    AVLNode<Key, Value> *nparent = node->getParent();
    AVLNode<Key, Value> *nleftright = nleft->getRight();
    nleft->setParent(nparent);
    if (nparent != nullptr)
    {
        if (nparent->getLeft() == node)
        {
            nparent->setLeft(nleft);
        }
        else
        {
            nparent->setRight(nleft);
        }
    }
    else
    {
        this->root_ = nleft;
    }

    node->setParent(nleft);
    node->setLeft(nleftright);
    if (nleftright != nullptr)
    {
        nleftright->setParent(node);
    }
    nleft->setRight(node);

    // Update balance factors
    node->setBalance(this->height(node->getLeft()) - this->height(node->getRight()));
    nleft->setBalance(this->height(nleft->getLeft()) - this->height(nleft->getRight()));
}

/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template <class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value> *current = dynamic_cast<AVLNode<Key, Value> *>(this->root_);

    if (current == nullptr)
    {
        current = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        current->setBalance(0);
        this->root_ = current;
        return;
    }
    while (current != nullptr)
    {
        if (new_item.first < current->getKey())
        {
            if (current->getLeft() == nullptr)
            {
                AVLNode<Key, Value> *newnode = new AVLNode<Key, Value>(new_item.first, new_item.second, current);
                current->setLeft(newnode);
                current = newnode;
                break;
            }
            else
            {
                current = current->getLeft();
            }
        }
        else if (new_item.first > current->getKey())
        {
            if (current->getRight() == nullptr)
            {
                AVLNode<Key, Value> *newnode = new AVLNode<Key, Value>(new_item.first, new_item.second, current);
                current->setRight(newnode);
                current = newnode;
                break;
            }
            else
            {
                current = current->getRight();
            }
        }
        else
        {
            current->setValue(new_item.second);
            return;
        }
    }
    while (current != nullptr)
    {
        int bal = this->height(current->getLeft()) - this->height(current->getRight());
        current->setBalance(bal);

        if (bal > 1)
        { // Left heavy
            if (new_item.first < current->getLeft()->getKey())
            {
                // Right rotation
                rightRotate(current);
            }
            else
            {
                // Left-Right rotation
                leftRotate(current->getLeft());
                rightRotate(current);
            }
            break;
        }
        else if (bal < -1)
        { // Right heavy
            if (new_item.first > current->getRight()->getKey())
            {
                // Left rotation
                leftRotate(current);
            }
            else
            {
                // Right-Left rotation
                rightRotate(current->getRight());
                leftRotate(current);
            }
            break;
        }

        current = current->getParent();
    }
}
template <class Key, class Value>
void AVLTree<Key, Value>::remove(const Key &key)
{
    remove(dynamic_cast<AVLNode<Key, Value> *>(this->root_), key);
    rebalancehelper(dynamic_cast<AVLNode<Key, Value> *>(this->root_));
    
}


template <class Key, class Value>
void AVLTree<Key, Value>::remove(AVLNode<Key, Value> *node, const Key &key)
{
    // if the tree is empty
    //make this return root
    if (node == nullptr)
        return;
    // Perform standard BST delete
    if (key < node->getKey())
        remove(node->getLeft(), key);
    else if (key > node->getKey())
        remove(node->getRight(), key);
    else
    {
        if (node->getLeft() != nullptr && node->getRight() != nullptr)
        {
            AVLNode<Key, Value> *pred = dynamic_cast<AVLNode<Key, Value> *>(this->predecessor(node));
            if (pred->getLeft() != nullptr){
                AVLNode<Key, Value>* lchildpred = pred->getLeft();
                if (pred->getParent() != node)
                {
                    pred->getParent()->setRight(lchildpred);
                    lchildpred->setParent(pred->getParent());
                }
            }

            nodeSwap(pred, node);
        }
        // Node with only one child or no child
        if (node->getLeft() == nullptr || node->getRight() == nullptr)
        {
            AVLNode<Key, Value> *temp = node->getLeft() ? node->getLeft() : node->getRight();
            AVLNode<Key, Value> *parent = node->getParent();
            //1 child
            if (temp != nullptr){
                temp->setParent(parent);
                if (parent != nullptr)
                {
                    if (parent->getRight() == node)
                    {
                        parent->setRight(temp);
                        node->setRight(nullptr);
                    }
                    else
                    {
                        parent->setLeft(temp);
                        node->setLeft(nullptr);
                    }
                }
                //singular leaf node
                else
                {
                    this->root_ = temp;
                    temp->setParent(nullptr);
                }
                /////^^^^
            }//0 children 

            else if (node->getLeft() == nullptr && node->getRight() == nullptr){
                if (parent != nullptr){
                    if (parent->getRight() == node)
                    {
                        parent->setRight(nullptr);
                    }
                    else{
                        parent->setLeft(nullptr);
                    }
                    
                }
                //no parent no children
                else
                {
                    this->root_ = nullptr;
                } 
            } 
            if (node->getLeft() == nullptr && node->getRight() == nullptr && parent != nullptr){
              delete node; 
              rebalance(parent);
              return;
            }
            else{
            delete node;

            return;
            }
        }
    }

    // If the tree had only one node then return
    if (node == nullptr)
    {
        return;
    }

    // Update height of the current node
    rebalance(node);
    return;
}

template <class Key, class Value>
void AVLTree<Key, Value>::rebalancehelper(AVLNode<Key, Value> *node)
{
  if (node == nullptr){
    return;
  }
  if (node->getLeft() != nullptr){
    rebalancehelper(node->getLeft());
    rebalance(node->getLeft());
  }
  if (node->getRight() != nullptr){
    rebalancehelper(node->getRight());
    rebalance(node->getRight());
  }
  rebalance(node);
  
}
template <class Key, class Value>
void AVLTree<Key, Value>::rebalance(AVLNode<Key, Value> *node)
{
    if (node == nullptr){
      return;
    }
    int bal = this->height(node->getLeft()) - this->height(node->getRight());
    while (node != nullptr)
    {
      
        bal = this->height(node->getLeft()) - this->height(node->getRight());
        node->setBalance(bal);
        while (abs(bal)> 1){
          if (bal > 1)
        { // Left heavy
            if (node->getLeft()->getBalance() >= 0)
            {
                // Right rotation
                rightRotate(node);
            }
            else
            {
                // Left-Right rotation
                leftRotate(node->getLeft());
                rightRotate(node);
            }
            return;
        }
        else if (bal < -1)
        { // Right heavy
            if (node->getRight()->getBalance() <= 0)
            {
                // Left rotation
                leftRotate(node);
            }
            else
            {
                // Right-Left rotation
                rightRotate(node->getRight());
                leftRotate(node);
            }
            return;
        }

        }
        bal = this->height(node->getLeft()) - this->height(node->getRight());
        node = node->getParent();
    }
}
template <class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key, Value> *n1, AVLNode<Key, Value> *n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

#endif
