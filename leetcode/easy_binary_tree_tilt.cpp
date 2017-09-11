#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
 
class Solution {
private:
	int traverse(TreeNode* root, int& tilt) {
		if (root == NULL) return 0;
		int left = traverse(root->left, tilt);
		int right = traverse(root->right, tilt);
		tilt += abs(left-right);
		return left+right+root->val;
	}
public:
    int findTilt(TreeNode* root) {
		int tilt = 0;
		traverse(root, tilt);
		return tilt;
	}
};

int main()
{
}
