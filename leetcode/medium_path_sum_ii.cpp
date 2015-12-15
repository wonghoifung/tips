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
	void pathSumHelper(TreeNode* root, int currentsum, int target, vector<int>& path, vector<vector<int>>& result) {
		if (root == NULL)
		{
			return;
		}
		int cur = currentsum + root->val;
		path.push_back(root->val);
		if (root->left == NULL && root->right == NULL && cur == target)
		{
			result.push_back(path);
		}
		if (root->left)
		{
			pathSumHelper(root->left, cur, target, path, result);
		}
		if (root->right)
		{
			pathSumHelper(root->right, cur, target, path, result);
		}
		path.pop_back();
	}
	vector<vector<int>> pathSum(TreeNode* root, int sum) {
		vector<vector<int>> ret;
		vector<int> path;
		pathSumHelper(root, 0, sum, path, ret);
		return ret;
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

