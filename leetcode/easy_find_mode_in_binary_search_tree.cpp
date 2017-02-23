#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
	void help(TreeNode* node, unordered_map<int, int>& counts) {
		if (node == NULL) return;
		counts[node->val] += 1;
		help(node->left, counts);
		help(node->right, counts);
	}
public:
	vector<int> findMode(TreeNode* root) {
		vector<int> res;
		unordered_map<int, int> counts;
		help(root, counts);
		int currentMax = INT_MIN;
		for (auto& p : counts) {
			if (p.second > currentMax) {
				res.clear();
				res.push_back(p.first);
				currentMax = p.second;
			}
			else if (p.second == currentMax) {
				res.push_back(p.first);
			}
		}
		return res;
	}
};

int main() {
	{
		TreeNode n1(1), n2(2), n3(2);
		n1.right = &n2;
		n2.left = &n3;
		auto res = Solution().findMode(&n1);
		for (int i : res) cout << i << " ";
		cout << endl;
	}
	{
		TreeNode n1(2147483647);
		auto res = Solution().findMode(&n1);
		for (int i : res) cout << i << " ";
		cout << endl;
	}
	cin.get();
}

