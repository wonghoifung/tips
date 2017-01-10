#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
	TreeNode* deleteNode(TreeNode* root, int key) {
		if (root == NULL) return NULL;
		TreeNode* parent = NULL;
		TreeNode* current = root;
		while (current && current->val != key) {
			parent = current;
			current = current->val < key ? current->right : current->left;
		}
		if (current == NULL) return root;
		if (current->left == NULL && current->right == NULL) { // leaf node
			if (root == current) return NULL; // delete root node
			if (parent->val < key) parent->right = NULL; // delete right child
			else parent->left = NULL; // delete left child
			return root;
		}
		if (current->left && current->right == NULL) {
			if (root == current) return current->left;
			if (parent->val < key) parent->right = current->left;
			else parent->left = current->left;
			return root;
		}
		if (current->left == NULL && current->right) {
			if (root == current) return current->right;
			if (parent->val < key) parent->right = current->right;
			else parent->left = current->right;
			return root;
		}
		TreeNode* leftChildRightMost = current->left;
		while (leftChildRightMost->right) leftChildRightMost = leftChildRightMost->right;
		leftChildRightMost->right = current->right->left;
		current->right->left = current->left;
		if (parent == NULL) return current->right;
		if (parent->val < key) parent->right = current->right;
		else parent->left = current->right;
		return root;
	}
};

int main() {

	cin.get();
	return EXIT_SUCCESS;
}

