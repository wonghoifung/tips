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
	bool dfs(TreeNode* root, vector<TreeNode*>& currentpath, TreeNode* val) {
		if (root == NULL)
		{
			return false;
		}

		currentpath.push_back(root);
		if (root == val)
		{
			return true;
		}

		if (dfs(root->left, currentpath, val))
		{
			return true;
		}
		if (dfs(root->right, currentpath, val))
		{
			return true;
		}

		currentpath.pop_back();
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
		vector<TreeNode*> ppath, qpath;
		if (dfs(root, ppath, p) && dfs(root, qpath, q))
		{
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
				return ppath.back();
			}
			if (ppath.size() > qpath.size())
			{
				return qpath.back();
			}
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
	{
		TreeNode n1(37), n2(-34), n3(-48), n4(-100), n5(-100), n6(48), n7(-54), n8(-71), n9(-22), n10(8);
		n1.left = &n2; n1.right = &n3;
		n2.right = &n4;
		n3.left = &n5; n3.right = &n6;
		n6.left = &n7;
		n7.left = &n8; n7.right = &n9;
		n9.right = &n10;
		TreeNode* lca = s.lowestCommonAncestor(&n1, &n4, &n8);
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

