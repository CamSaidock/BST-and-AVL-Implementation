#include "equal-paths.h"
#include <iostream>
using namespace std;


// You may add any prototypes of helper functions here

int calculateHeight(Node* root) {
	// Base case: an empty tree is always balanced and has a height of 0
	if (root == nullptr) return 0;

	// Think about cases to handle:
	// - What if at least one subtree is NOT balanced?
	// - What if BOTH are balanced?

	// TODO: handle the cases here
	return max(calculateHeight(root->right), calculateHeight(root->left)) + 1;

}

bool equalPaths(Node* root)
{
	if (root == NULL) {
		return true;
	}

	if(calculateHeight(root) == 2) {
		return true;
	}	

	if(root->left == nullptr && root->right == nullptr) {
		return true;
	}

	else {
		return (calculateHeight(root->left) == calculateHeight(root->right));
	}

	if(root->left == nullptr) {
		return equalPaths(root->right);
	}

	if(root->right == nullptr) {
		return equalPaths(root->left);
	}

	return (calculateHeight(root->left) == calculateHeight(root->right));
	
}
