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
	bool dfs(TreeNode* root, vector<TreeNode*>& path, int val) {
		if (root == NULL)
		{
			return false;
		}
		if (root->val == val)
		{
			path.push_back(root);
			return true;
		}
		path.push_back(root);
		if (dfs(root->left, path, val))
		{
			return true;
		}
		if (dfs(root->right, path, val))
		{
			return true;
		}
		path.pop_back();
		return false;
	}
	TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
		if (root == NULL || p == NULL || q == NULL)
		{
			return NULL;
		}
		if (p == q)
		{
			return p;
		}
		if (p == root || q == root)
		{
			return root;
		}
		vector<TreeNode*> ppath;
		vector<TreeNode*> qpath;
		if (!dfs(root, ppath, p->val) || !dfs(root, qpath, q->val))
		{
			return NULL;
		}
		size_t i = 0;
		for (; i < ppath.size() && i < qpath.size(); ++i)
		{
			if (ppath[i] != qpath[i])
			{
				return ppath[i - 1];
			}
		}
		if (ppath.size() < qpath.size())
		{
			return p;
		}
		if (ppath.size() > qpath.size())
		{
			return q;
		}
		return NULL;
	}
};

int main()
{
	Solution s;
	{
		TreeNode n1(1), n2(2), n3(3), n4(4), n5(5), n6(6);
		TreeNode* root = &n5;
		root->left = &n3;
		root->right = &n6;
		n3.left = &n2;
		n3.right = &n4;
		n2.left = &n1;
		TreeNode* lca = s.lowestCommonAncestor(root, &n1, &n3);
		if (lca)
		{
			cout << "found: " << lca->val << endl;
		}
		else
		{
			cout << "not found" << endl;
		}
	}
	std::cin.get();

	return 0;
}

