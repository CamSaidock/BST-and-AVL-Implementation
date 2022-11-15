#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

using namespace std;

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
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
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
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		virtual void rotateLeft(AVLNode<Key, Value>* current);
		virtual void rotateRight(AVLNode<Key, Value>* current);
		virtual void insertFix(AVLNode<Key, Value>* current, AVLNode<Key, Value>* parent);
		virtual void removeFix(AVLNode<Key, Value>* current, int8_t diff);
		AVLNode<Key, Value>* internalFind(const Key& key) const;
		AVLNode<Key, Value>* successor(AVLNode<Key, Value>* current);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* current) {
	AVLNode<Key, Value>* temp = (current->getRight());
	AVLNode<Key, Value>* tempChild = (temp->getLeft());

	if(current->getParent() != nullptr) {
		AVLNode<Key, Value>* currentParent = current->getParent();

		if(currentParent->getRight() == current) {
			currentParent->setRight(temp);
		}
		else if(currentParent->getLeft() == current) {
			currentParent->setLeft(temp);
		}
	}

	else if(current->getParent() == nullptr) {
		this->root_ = temp;
	}

	temp->setParent(current->getParent());
	temp->setLeft(current);
	current->setRight(tempChild);
	if(tempChild != nullptr) {
		tempChild->setParent(current);
	}
	current->setParent(temp);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* current) {
	AVLNode<Key, Value>* temp = (current->getLeft());
	AVLNode<Key, Value>* tempChild = (temp->getRight());

	if(current->getParent() != nullptr) {
		AVLNode<Key, Value>* currentParent = current->getParent();

		if(currentParent->getRight() == current) {
			currentParent->setRight(temp);
		}
		else if(currentParent->getLeft() == current) {
			currentParent->setLeft(temp);
		}
	}

	else if(current->getParent() == nullptr) {
		this->root_ = temp;
	}

	temp->setParent(current->getParent());
	temp->setRight(current);
	current->setLeft(tempChild);
	if(tempChild != nullptr) {
		tempChild->setParent(current);
	}
	current->setParent(temp);
}



template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* current, AVLNode<Key, Value>* parent)
{
	if((parent == nullptr) || (parent->getParent() == nullptr)) {
		return;
	}

	AVLNode<Key, Value>* grandParent = parent->getParent();

	if(parent->getLeft() == current) {
		grandParent->updateBalance(-1);
		if(grandParent->getBalance() == 0) {
			return;
		}

		else if(grandParent->getBalance() == -1) {
			insertFix(parent, grandParent);
		}

		else if(grandParent->getBalance() == -2) {
			if(parent->getBalance() == -1) {
				rotateRight(grandParent);
				parent->setBalance(0);
				grandParent->setBalance(0);
			}

			else if(parent->getBalance() == 1) {
				rotateLeft(parent);
				rotateRight(grandParent);

				if(current->getBalance() == -1) {
					parent->setBalance(0);
					grandParent->setBalance(1);
					current->setBalance(0);
				}
				else if(current->getBalance() == 0) {
					parent->setBalance(0);
					grandParent->setBalance(0);
					current->setBalance(0);
				}
				else if(current->getBalance() == 1) {
					parent->setBalance(-1);
					grandParent->setBalance(0);
					current->setBalance(0);
				}
			}
		}
	}
	
	else if(parent->getRight() == parent) { //Parent is right child of grandParent
		grandParent->updateBalance(1);
		if(grandParent->getBalance() == 0) {
			return;
		}

		else if(grandParent->getBalance() == 1) {
			insertFix(parent, grandParent);
		}

		else if(grandParent->getBalance() == 2) {
			if(parent->getBalance() == 1) {
				rotateLeft(grandParent);
				parent->setBalance(0);
				grandParent->setBalance(0);
			}

			else if(parent->getBalance() == -1) {
				rotateRight(parent);
				rotateLeft(grandParent);

				if(current->getBalance() == 1) {
					parent->setBalance(0);
					grandParent->setBalance(-1);
					current->setBalance(0);
				}
				else if(current->getBalance() == 0) {
					parent->setBalance(0);
					grandParent->setBalance(0);
					current->setBalance(0);
				}
				else if(current->getBalance() == -1) {
					parent->setBalance(1);
					grandParent->setBalance(0);
					current->setBalance(0);
				}
			}
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  // TODO
	if(this->root_ == nullptr) {
		this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
		this->root_->setLeft(nullptr);
		this->root_->setRight(nullptr);
		return;
	}

	AVLNode<Key, Value>* nodeFind = internalFind(new_item.first);
	if(nodeFind == nullptr) {
		delete nodeFind;
	}
	else {
		nodeFind->setValue(new_item.second);
		return;
	}

	AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
	AVLNode<Key, Value>* parent = current;

	while(current != nullptr) {
		if(new_item.first > current->getKey()) {
			parent = current;
			current = current->getRight();
		}

		else if(new_item.first < current->getKey()) {
			parent = current;
			current = current->getLeft();
		}
	}

	AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
	if(new_item.first < parent->getKey()) {
		parent->setLeft(newNode);
		newNode->setParent(parent);
	}
	else if(new_item.first > parent->getKey()) {
		parent->setRight(newNode);
		newNode->setParent(parent);
	}

	if(parent->getBalance() == -1) {
		parent->setBalance(0);
	}

	else if(parent->getBalance() == 1) {
		parent->setBalance(0);
	}

	else if(parent->getBalance() == 0) {
		if(parent->getRight() == newNode) {
			parent->setBalance(1);
			cout << "Insert Fix 1" << endl;
			insertFix(newNode, parent);
		}
		else if(parent->getLeft() == newNode) {
			parent->setBalance(-1);
			cout << "Insert Fix 2" << endl;
			insertFix(newNode, parent);
		}
	}

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* current, int8_t diff) //Look at balance later
{
	std::cout << "Remove Fix!" << endl;
	if(current == nullptr) {
		return;
	}

	AVLNode<Key, Value>* parent = current->getParent();
	int8_t ndiff;

	if(current->getParent() != nullptr) {
		if(parent->getLeft() == current) {
			ndiff = 1;
		}
		else if(parent->getRight() == current) {
			ndiff = -1;
		}
	}

	//Case 1, diff = -1
	if(current->getBalance() + diff == -2) {
		AVLNode<Key, Value>* left = current->getLeft();

		if(left->getBalance() == -1) {
			rotateRight(current);
			current->setBalance(0);
			left->setBalance(0);
			removeFix(parent, ndiff);
		}

		else if(left->getBalance() == 0) {
			rotateRight(current);
			current->setBalance(-1);
			left->setBalance(1);
		}

		else if(left->getBalance() == 1) {
			AVLNode<Key, Value>* rightOfLeft = left->getRight();

			rotateLeft(left);
			rotateRight(current);

			if(rightOfLeft->getBalance() == 1) {
				current->setBalance(0);
				left->setBalance(-1);
				rightOfLeft->setBalance(0);
			}
			else if(rightOfLeft->getBalance() == 0) {
				current->setBalance(0);
				left->setBalance(0);
				rightOfLeft->setBalance(0);
			}
			else if(rightOfLeft->getBalance() == -1) {
				current->setBalance(1);
				left->setBalance(0);
				rightOfLeft->setBalance(0);
			}
			removeFix(parent, ndiff);
		}
	}

	else if(current->getBalance() + diff == -1) {
		current->setBalance(-1);
	}

	else if(current->getBalance() + diff == 0) {
		current->setBalance(0);
		removeFix(parent, ndiff);
	}

	//Case 2
	else if(current->getBalance() + diff == 2) {
		AVLNode<Key, Value>* right = current->getRight();

		if(right->getBalance() == 1) {
			rotateLeft(current);
			current->setBalance(0);
			right->setBalance(0);
			removeFix(parent, ndiff);
		}

		else if(right->getBalance() == 0) {
			rotateLeft(current);
			current->setBalance(1);
			right->setBalance(-1);
		}

		else if(right->getBalance() == -1) {
			AVLNode<Key, Value>* leftOfRight = right->getLeft();
			
			rotateRight(right);
			rotateLeft(current);

			if(leftOfRight->getBalance() == -1) {
				current->setBalance(0);
				right->setBalance(1);
				leftOfRight->setBalance(0);
			}
			else if(leftOfRight->getBalance() == 0) {
				current->setBalance(0);
				right->setBalance(0);
				leftOfRight->setBalance(0);
			}
			else if(leftOfRight->getBalance() == 1) {
				current->setBalance(-1);
				right->setBalance(0);
				leftOfRight->setBalance(0);
			}
			removeFix(parent, ndiff);
		}
	}
	
	else if(current->getBalance() + diff == 1) {
		current->setBalance(1);
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  // TODO
	std::cout << "Removing!" << endl;
	if(this->root_ == nullptr) {
		return;
	}

	AVLNode<Key, Value>* findNode = internalFind(key);
	if(findNode == nullptr) {
		return;
	}

	else if(findNode->getRight() != nullptr && findNode->getLeft() != nullptr) { //Node has children
		AVLNode<Key, Value>* successorNode = successor(findNode);
		nodeSwap(successorNode, findNode);
	}

	AVLNode<Key, Value>* parent = findNode->getParent();
	int8_t diff;
	if(parent != nullptr) {
		if(parent->getLeft() == findNode) {
			diff = 1;
		}
		else if(parent->getRight() == findNode) {
			diff = -1;
		}
	}

	if(findNode->getLeft() == nullptr && findNode->getRight() == nullptr) {
		if(parent->getLeft() == findNode) {
			parent->setLeft(nullptr);
			delete findNode;
		}
		else if(parent->getRight() == findNode) {
			parent->setRight(nullptr);
			delete findNode;
		}
	}

	else if(findNode->getLeft() != nullptr || findNode->getRight() != nullptr) { //Node has 1 child
		if(parent == nullptr) {
			if(findNode->getLeft() == nullptr) {
				findNode->getRight()->setParent(nullptr);
				this->root_ = findNode->getRight();
				delete findNode;
			}
			else if(findNode->getRight() == nullptr) {
				findNode->getLeft()->setParent(nullptr);
				this->root_ = findNode->getLeft();
				delete findNode;
			}
		}

		else if(parent->getLeft() == findNode) { //Node is left child of parent
			if(findNode->getLeft() == nullptr) { //Node's child is right child
				parent->setLeft(findNode->getRight());
				findNode->getRight()->setParent(parent);
			}
			else if(findNode->getRight() == nullptr) { //Node's child is left child
				parent->setLeft(findNode->getLeft());
				findNode->getLeft()->setParent(parent);
			}
		}

		else if(parent->getRight() == findNode) { //Node is right child of parent
			if(findNode->getLeft() == nullptr) { //Node's child is right child
				parent->setRight(findNode->getRight());
				findNode->getRight()->setParent(parent);
			}
			else if(findNode->getRight() == nullptr) { //Node's child is left child
				parent->setRight(findNode->getLeft());
				findNode->getLeft()->setParent(parent);
			}
		}

		delete findNode;
	}

	removeFix(parent, diff); 
	
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::successor(AVLNode<Key, Value>* current)
{   
    AVLNode<Key, Value>* itr = current;

		if(current->getLeft() == nullptr && current->getRight() == nullptr) {
			return nullptr;
		}
    
    if(current->getRight() != nullptr) { //Make sure that the current node has a right subtree
			itr = current->getRight();
			while(itr->getLeft() != nullptr) {
					itr = itr->getLeft();
			}
			return itr;		
    }

		else if(itr->getRight() == nullptr) {
			itr = current;
			AVLNode<Key, Value>* itrParent;
			while(itr->getParent() != nullptr) {
				itrParent = itr->getParent();
				if(itr == itrParent->getLeft()) {
					return itrParent;
				}
				itr = itr->getParent();
			}
		}

		return itr;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::internalFind(const Key& key) const
{
	AVLNode<Key, Value>* itr = static_cast<AVLNode<Key, Value>*>(this->root_);

	if(itr == nullptr) { //Empty tree, return NULL
		return nullptr;
	}

	else {
		while(itr != nullptr) {
			if(key == itr->getKey()) { //If node has been found, return the node
				return itr;
			}

			else if(key < itr->getKey()) { //If the node key is less than the iterator's key, search through left subtree
				itr = itr->getLeft();
			}

			else if(key > itr->getKey()) { //If the node key is greater than the iterator's key, search through right subtree
				itr = itr->getRight();
			}
		}
	}

	return itr;
}

#endif
