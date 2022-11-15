#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

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
		AVLNode<Key, Value>* predecessor(AVLNode<Key, Value>* current);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* current) {
	AVLNode<Key, Value>* temp = (current->getRight());
	AVLNode<Key, Value>* tempChild = (temp->getLeft());

	//Left rotation, taking right child and making it parent and making original parent new left child

	if(current->getParent() != nullptr) { //If the current node isn't a root_
		AVLNode<Key, Value>* currentParent = current->getParent();

		if(currentParent->getRight() == current) { //If current is right node of parent
			currentParent->setRight(temp);
			temp->setParent(current->getParent());
		}
		else if(currentParent->getLeft() == current) { //If current is left node of parent
			currentParent->setLeft(temp);
			temp->setParent(current->getParent());
		}

		temp->setLeft(current);
		current->setRight(tempChild);
		current->setParent(temp);
	}

	else if(current->getParent() == nullptr) { //If current's parent is nullptr, set temp as new root
		this->root_ = temp;
		temp->setParent(nullptr);
		temp->setLeft(current);
		current->setRight(tempChild);
		current->setParent(temp);
	}

	if(tempChild != nullptr) {
		tempChild->setParent(current);
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* current) {
	AVLNode<Key, Value>* temp = (current->getLeft());
	AVLNode<Key, Value>* tempChild = (temp->getRight());

	//Right rotation, taking left child and making it parent and making original parent new right child

	if(current->getParent() != nullptr) { //If the current node isn't a root_
		AVLNode<Key, Value>* currentParent = current->getParent();

		if(currentParent->getRight() == current) { //If current is right node of parent
			currentParent->setRight(temp);
			temp->setParent(currentParent);
		}
		else if(currentParent->getLeft() == current) { //If current is left node of parent
			currentParent->setLeft(temp);
			temp->setParent(currentParent);
		}

		temp->setRight(current);
		current->setLeft(tempChild);
		current->setParent(temp);
	}

	else if(current->getParent() == nullptr) { //If current's parent is nullptr, set temp as new root
		this->root_ = temp;
		temp->setParent(nullptr);
		temp->setRight(current);
		current->setLeft(tempChild);
		current->setParent(temp);
	}

	if(tempChild != nullptr) {
		tempChild->setParent(current);
	}
}



template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* current, AVLNode<Key, Value>* parent)
{
	if((parent == nullptr) || (parent->getParent() == nullptr)) {
		return;
	}

	AVLNode<Key, Value>* grandParent = parent->getParent();

	if(grandParent->getLeft() == parent) { //Current node is left node of parent
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
	
	else if(grandParent->getRight() == parent) { //Current node is right node of parent
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
		Node<Key, Value>* newRoot = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
		this->root_ = newRoot;
		return;
	}

	AVLNode<Key, Value>* nodeFind = internalFind(new_item.first); //First, check to see if node already exists
	if(nodeFind != nullptr) {
		nodeFind->setValue(new_item.second);
		return;
	}

	else {
		delete nodeFind;
		AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
		AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(this->root_); //Keep track of parent in case itr becomes null

		if(new_item.first < current->getKey()) { //Check if the key is smaller than current's key
			if(current->getLeft() != nullptr) { //If root_ contains a left child, search through left subtree
				current = current->getLeft();
			}
			
			while(current != nullptr) { //Search through the subtree until current becomes nullptr
				if(new_item.first < current->getKey()) { //If key is smaller than current, continue searching through left subtree
					parent = current;
					current = current->getLeft();
				}
				else if(new_item.first > current->getKey()) { //If key is greater than current, continue searching through right subtree
					parent = current;
					current = current->getRight();
				}
			}
		}

		else if(new_item.first > current->getKey()) { //Check if the key is greater than current's key
			if(current->getRight() != nullptr) { //If root_ contains a left child, search through left subtree
				current = current->getRight();
			}

			while(current != nullptr) { //Search through the subtree until current becomes nullptr
				if(new_item.first < current->getKey()) { //If key is smaller than current, continue searching through left subtree
					parent = current;
					current = current->getLeft();
				}

				else if(new_item.first > current->getKey()) { //If key is greater than current, continue searching through right subtree
					parent = current;
					current = current->getRight();
				}
			}
		}

		//Create new node based on parent's location

		AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
		if(new_item.first < parent->getKey()) { //If key is less than parent make it a left node
			parent->setLeft(newNode);
			newNode->setBalance(0);
		}
		else if(new_item.first > parent->getKey()) { //If key is greater than parent make it a right node
			parent->setRight(newNode); 
			newNode->setBalance(0);
		}

		//Set balances

		if(parent->getBalance() == -1) {
			parent->setBalance(0);
		}

		else if(parent->getBalance() == 1) {
			parent->setBalance(0);
		}

		else if(parent->getBalance() == 0) {
			if(parent->getRight() == newNode) {
				parent->setBalance(1);
			}
			else if(parent->getLeft() == newNode) {
				parent->setBalance(-1);
			}
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
	if(current == nullptr) { //If current doesn't exist, return
		return;
	}

	AVLNode<Key, Value>* parent = current->getParent(); //Keep track of current's parent for next recursive call
	int8_t ndiff; //Keep track of ndiff for next recursive call

	if(parent != nullptr) {
		if(parent->getLeft() == current) { //Current is left child
			ndiff = 1;
		}
		else if(parent->getRight() == current) { //Current is left child
			ndiff = -1;
		}
	}

	if(current->getBalance() + diff == -2) {
		AVLNode<Key, Value>* child = current->getLeft();
		if(child->getBalance() == -1) {
			rotateRight(current); //Zig-zig
			current->setBalance(0);
			child->setBalance(0);
			removeFix(parent, ndiff);
		}
		else if(child->getBalance() == 0) {
			rotateRight(current); //Zig-zig
			current->setBalance(-1);
			child->setBalance(1);
			return; //Done!
		}
		else if(child->getBalance() == 1) {
			AVLNode<Key, Value>* grandchild = child->getRight();
			rotateLeft(child);
			rotateRight(current); //Zig-zag
			if(grandchild->getBalance() == 1) {
				current->setBalance(0);
				child->setBalance(-1);
				grandchild->setBalance(0);
			}

			else if(grandchild->getBalance() == 0) {
				current->setBalance(0);
				child->setBalance(0);
				grandchild->setBalance(0);
			}

			else if(grandchild->getBalance() == -1) {
				current->setBalance(1);
				child->setBalance(0);
				grandchild->setBalance(0);
			}

			removeFix(parent, ndiff);
		}
	}

	else if(current->getBalance() + diff == -1) {
		current->setBalance(-1);
		return;
	}

	else if(current->getBalance() + diff == 0) {
		current->setBalance(0);
		removeFix(parent, ndiff);
	}

	else if(current->getBalance() + diff == 2) {
		AVLNode<Key, Value>* child = current->getRight();
		if(child->getBalance() == 1) {
			rotateLeft(current); //Zig-zig
			current->setBalance(0);
			child->setBalance(0);
			removeFix(parent, ndiff);
		}
		else if(child->getBalance() == 0) {
			rotateLeft(current); //Zig-zig
			current->setBalance(1);
			child->setBalance(-1);
		}
		else if(child->getBalance() == -1) {
			AVLNode<Key, Value>* grandchild = child->getLeft();
			rotateRight(child);
			rotateLeft(current); //Zig-zag
			if(grandchild->getBalance() == -1) {
				current->setBalance(0);
				child->setBalance(1);
				grandchild->setBalance(0);
			}

			else if(grandchild->getBalance() == 0) {
				current->setBalance(0);
				child->setBalance(0);
				grandchild->setBalance(0);
			}

			else if(grandchild->getBalance() == 1) {
				current->setBalance(-1);
				child->setBalance(0);
				grandchild->setBalance(0);
			}

			removeFix(parent, ndiff);
		}
	}

	else if(current->getBalance() + diff == 1) {
		current->setBalance(1);
		return;
	}

}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  // TODO
	AVLNode<Key, Value>* current = internalFind(key); //Find node to remove

	if(current == nullptr || this->root_ == nullptr) { //If node doesn't exist then return.
		return;
	}

	if(current->getRight() != nullptr && current->getLeft() != nullptr) { //If node has two children, then swap with predecessor
		AVLNode<Key, Value>* predecessorNode = predecessor(current);
		nodeSwap(current, predecessorNode);
	}

	AVLNode<Key, Value>* parent = current->getParent(); //Keep track of parent of current
	int8_t diff = 0;


	if(parent != nullptr) { //Update diff for removeFix
		if(current == parent->getLeft()) {
			diff = 1;
		}
		else if(current == parent->getRight()) {
			diff = -1;
		}
	}

	AVLNode<Key, Value>* child; //Create a child node to keep track of current node's only child if it has less than two
	if(current->getRight() != nullptr) { //If current only contains a right node
		child = current->getRight();
	}

	else { //If current only contains a left node
		child = current->getLeft();
	}

	if(parent == nullptr) { //If current is root then set child has new root 
		this->root_ = child;
		if(child != nullptr) {
			child->setParent(nullptr);
		}
	}

	else {
		if(current == parent->getLeft()) {
			parent->setLeft(child);
		}
		else if(current == parent->getRight()) {
			parent->setRight(child);
		}

		if(child != nullptr) {
			child->setParent(parent);
		}
	}

	delete current;
	removeFix(parent, diff);
	
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* current)
{   
	AVLNode<Key, Value>* itr = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(current));
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
	AVLNode<Key, Value>* itr = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
	return itr;
}

#endif
