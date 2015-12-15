#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <memory>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

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
	bool hasPathSumHelper(TreeNode* root, int currentsum, int target) {
		if (root == NULL)
		{
			return currentsum == target;
		}
		int cur = root->val + currentsum;
		if (root->left == NULL && root->right == NULL)
		{
			return cur == target;
		}
		if (root->left && hasPathSumHelper(root->left, cur, target))
		{
			return true;
		}
		return root->right && hasPathSumHelper(root->right, cur, target);
	}
	bool hasPathSum(TreeNode* root, int sum) {
		if (root == NULL) return false;
		return hasPathSumHelper(root, 0, sum);
	}
};

int main()
{
	Solution s;
	{

	}

	std::cin.get();
	return 0;
}

