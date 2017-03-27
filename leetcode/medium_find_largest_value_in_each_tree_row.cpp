#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>
using namespace std;
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
public:
    vector<int> largestValues(TreeNode* root) {
        vector<int> res;
		if (root==NULL) return res;
		queue<TreeNode*> q;
		q.push(root);
		while (!q.empty()) {
			int len = q.size();
			int largest = INT_MIN;
			for (int i=0; i<len; ++i) {
				TreeNode* cur = q.front();
				q.pop();
				if (cur->val > largest) largest = cur->val;
				if (cur->left) q.push(cur->left);
				if (cur->right) q.push(cur->right);
			}
			res.push_back(largest);
		}
		return res;
    }
};
int main() {
	TreeNode n1(1),n2(3),n3(2),n4(5),n5(3),n6(9);
	n1.left=&n2, n1.right=&n3;
	n2.left=&n4, n2.right=&n5;
	n3.right=&n6;
	vector<int> res = Solution().largestValues(&n1);
	for (int i=0; i<res.size(); ++i) cout<<res[i]<<" ";
	cout<<endl;
}

