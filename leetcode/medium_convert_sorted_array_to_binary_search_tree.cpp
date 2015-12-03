#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <deque>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>

using namespace std;

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(NULL);
	}
	~mytimer()
	{
		time_t end = time(NULL);
		cout << end - start_ << "s" << endl;
	}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
	TreeNode* help_build(vector<int>& nums, int start, int end) {
		if (nums.empty() || start > end || start < 0 || end >= nums.size() || start >= nums.size() || end < 0) {
			return NULL;
		}
		if (start == end) {
			TreeNode* root = new TreeNode(nums[start]);
			root->left = NULL;
			root->right = NULL;
			return root;
		}
		int rootidx = (start + end) / 2;
		TreeNode* root = new TreeNode(nums[rootidx]);
		root->left = help_build(nums, start, rootidx - 1);
		root->right = help_build(nums, rootidx + 1, end);
		return root;
	}

	TreeNode* sortedArrayToBST(vector<int>& nums) {
		return help_build(nums, 0, nums.size() - 1);
	}
};

void print_tree(TreeNode* t)
{
	if (t)
	{
		print_tree(t->left);
		cout << t->val << " ";
		print_tree(t->right);
	}
}

int main()
{
	vector<int> nums{ -1, 0, 1, 2 };
	Solution s;
	TreeNode* t = s.sortedArrayToBST(nums);

	print_tree(t);
	cout << endl;

	std::cin.get();

	return 0;
}

