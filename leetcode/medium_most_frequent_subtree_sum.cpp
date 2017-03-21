#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
using namespace std; 
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 };
class Solution {
	void help(TreeNode* root, unordered_map<TreeNode*,int>& dp, unordered_map<int,int>& dp2, int& maxcount, unordered_set<int>& res) {
		if (root && root->left == NULL && root->right == NULL) {
			dp[root] = root->val;
			dp2[root->val] += 1;
			if (dp2[root->val] > maxcount) {
				res.clear();
				maxcount = dp2[root->val];
				res.insert(root->val);
			} else if (dp2[root->val] == maxcount) {
				res.insert(root->val);
			}
			return;
		}
		if (root && root->left) help(root->left, dp, dp2, maxcount, res);
		if (root && root->right) help(root->right, dp, dp2, maxcount, res);
		dp[root] = root->val + dp[root->left] + dp[root->right];
		dp2[dp[root]] += 1;
		if (dp2[dp[root]] > maxcount) {
			res.clear();
			maxcount = dp2[dp[root]];
			res.insert(dp[root]);
		} else if (dp2[dp[root]] == maxcount) {
			res.insert(dp[root]);
		}
	}
public:
    vector<int> findFrequentTreeSum(TreeNode* root) {
		if (root == NULL) return vector<int>();
        unordered_map<TreeNode*,int> dp;
		unordered_map<int,int> dp2;	
		int maxcount = 0;
		unordered_set<int> res;
		help(root, dp, dp2, maxcount, res);
		return vector<int>(res.begin(),res.end());
    }
};
int main() {
	{
	TreeNode n1(5),n2(2),n3(-3);
	n1.left=&n2;
	n1.right=&n3;
	auto res = Solution().findFrequentTreeSum(&n1);
	for (auto i = 0; i<res.size(); ++i) {cout << res[i] << " ";}
	cout<<endl;
	}
	{
	TreeNode n1(5),n2(2),n3(-5);
	n1.left=&n2;
	n1.right=&n3;
	auto res = Solution().findFrequentTreeSum(&n1);
	for (auto i = 0; i<res.size(); ++i) {cout << res[i] << " ";}
	cout<<endl;
	}
}

