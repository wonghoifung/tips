#include <limits.h>
#include <iostream>
using namespace std;
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
    int diff;
	int* prev;
public:
	Solution():diff(INT_MAX),prev(NULL){}
    int getMinimumDifference(TreeNode* root) {
		if (root==NULL) return diff;
		getMinimumDifference(root->left);
		if (prev != NULL) diff = min(diff, root->val-*prev);
		prev = &(root->val);
		getMinimumDifference(root->right);
		return diff;    
    }
};
int main() {
	TreeNode n1(1),n2(3),n3(2);
	n1.right=&n2;
	n2.left=&n3;
	cout<<Solution().getMinimumDifference(&n1)<<endl;
}
