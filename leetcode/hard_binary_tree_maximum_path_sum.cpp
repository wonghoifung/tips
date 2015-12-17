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
	int maxBranch(TreeNode* root, int& maxsum) {
		if (root == NULL) return 0;
		int left = max(maxBranch(root->left, maxsum), 0);
		int right = max(maxBranch(root->right, maxsum), 0);
		maxsum = max(maxsum, root->val + left + right);
		return root->val + max(left, right);
	}

	int maxPathSum(TreeNode* root) {
		if (root == NULL) return 0;
		int maxsum = INT_MIN;
		maxBranch(root, maxsum);
		return maxsum;
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

