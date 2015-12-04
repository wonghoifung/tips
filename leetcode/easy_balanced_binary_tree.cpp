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
	int height(TreeNode* root) {
		if (root == NULL)
		{
			return 0;
		}
		return max(height(root->left), height(root->right)) + 1;
	}
	bool isBalanced(TreeNode* root) {
		if (root == NULL)
		{
			return true;
		}
		if (!isBalanced(root->left))
		{
			return false;
		}
		if (!isBalanced(root->right))
		{
			return false;
		}
		int leftheight = height(root->left);
		int rightheight = height(root->right);
		return abs(leftheight - rightheight) <= 1;
	}
};

int main()
{
	

	std::cin.get();

	return 0;
}

