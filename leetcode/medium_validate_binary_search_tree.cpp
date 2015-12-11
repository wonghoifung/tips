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
	bool inorder(TreeNode* root, long long& last) {
		if (root == NULL)
		{
			return true;
		}
		if (!inorder(root->left, last))
		{
			return false;
		}
		if (root->val <= last)
		{
			return false;
		}
		last = root->val;
		return inorder(root->right, last);
	}
	bool isValidBST(TreeNode* root) {
		if (root == NULL || (root->left == NULL && root->right == NULL))
		{
			return true;
		}
		long long last = LLONG_MIN;
		return inorder(root, last);
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

