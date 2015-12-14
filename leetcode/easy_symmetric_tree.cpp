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
#if 0
class Solution {
public:
	void inorder(TreeNode* root, vector<int>& nodes) {
		if (root)
		{
			inorder(root->left, nodes);
			nodes.push_back(root->val);
			inorder(root->right, nodes);
		}
	}
	bool isSymmetric(TreeNode* root) {
		if (root == NULL || (root->left == NULL && root->right == NULL))
		{
			return true;
		}
		if (!(root->left != NULL && root->right != NULL))
		{
			return false;
		}
		if (root->left->val != root->right->val)
		{
			return false;
		}
		vector<int> nodes;
		inorder(root, nodes);
		int len = nodes.size();
		int halflen = len / 2;
		for (int i = 0; i < halflen; ++i)
		{
			if (nodes[i] != nodes[len - 1 - i])
			{
				return false;
			}
		}
		return true;
	}
};
#endif

class Solution {
public:
	bool sym(TreeNode* l, TreeNode* r) {
		if (!l || !r)
		{
			return l == r;
		}
		if (l->val != r->val)
		{
			return false;
		}
		if (!sym(l->left, r->right))
		{
			return false;
		}
		if (!sym(l->right, r->left))
		{
			return false;
		}
		return true;
	}
	bool isSymmetric(TreeNode* root) {
		if (root == NULL)
		{
			return true;
		}
		return sym(root->left, root->right);
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

