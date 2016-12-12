#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
public:
	int sumOfLeftLeaves(TreeNode* root) {
		if (root == NULL) return 0;
		int sumOfRoot = 0;
		if (root->left == NULL) {
			sumOfRoot = sumOfLeftLeaves(root->right);
			return sumOfRoot;
		}
		if (root->left->left == NULL && root->left->right == NULL) {
			sumOfRoot = root->left->val + sumOfLeftLeaves(root->right);
			return sumOfRoot;
		}
		sumOfRoot = sumOfLeftLeaves(root->left) + sumOfLeftLeaves(root->right);
		return sumOfRoot;
	}
};

int main() {
	

	cin.get();
	return EXIT_SUCCESS;
}

