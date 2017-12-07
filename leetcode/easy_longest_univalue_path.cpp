#include <iostream>
#include <algorithm>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
 
class Solution {
	int dfs(TreeNode* root, int& len) {
		int l = root->left ? dfs(root->left, len) : 0;
		int r = root->right ? dfs(root->right, len) : 0;
		int resl = root->left && root->left->val == root->val ? l + 1 : 0;
		int resr = root->right && root->right->val == root->val ? r + 1 : 0;
		len = max(len, resl + resr);
		return max(resl, resr);
	}
public:
    int longestUnivaluePath(TreeNode* root) {
    	int longest = 0;
    	if (root) dfs(root, longest);
    	return longest;
    }
};

int main() {

}
