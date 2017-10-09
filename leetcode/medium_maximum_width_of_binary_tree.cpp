#include <iostream>
#include <deque>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    int widthOfBinaryTree(TreeNode* root) {
    	if (root == NULL) return 0;
		int maxWidth = 0;
		std::deque<TreeNode*> q;
		q.push_back(root);
		while (!q.empty()) {
			int len = q.size();
			maxWidth = len > maxWidth ? len : maxWidth;
			for (int i=0; i<len; ++i) {
				TreeNode* n = q.front();
				q.pop_front();
				if (n) {
					q.push_back(n->left);
					q.push_back(n->right);
				} else {
					q.push_back(NULL);
					q.push_back(NULL);
				}
			}
			while (!q.empty()) {
				if (q.back() == NULL) q.pop_back();
				else break;
			}
			while (!q.empty()) {
				if (q.front() == NULL) q.pop_front();
				else break;
			}
		}
		return maxWidth;
    }
};

int main() {

}
