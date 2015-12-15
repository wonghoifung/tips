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
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
	TreeNode* errnode;
	void inorder(TreeNode* root, vector<TreeNode*>& nodes) {
		if (root == NULL)
		{
			return;
		}
		inorder(root->left, nodes);
		if (nodes.size() > 0 && root->val < nodes.back()->val)
		{
			errnode = root;
		}
		nodes.push_back(root);
		inorder(root->right, nodes);
	}
	void recoverTree(TreeNode* root) {
		errnode = NULL;
		vector<TreeNode*> nodes;
		inorder(root, nodes);
		if (errnode)
		{
			for (size_t i = 0; i < nodes.size(); ++i)
			{
				if (nodes[i]->val < errnode->val)
				{
					continue;
				}
				int tmp = nodes[i]->val;
				nodes[i]->val = errnode->val;
				errnode->val = tmp;
				return;
			}
		}
	}
};

int main()
{
	Solution s;
	{
		TreeNode n1(2), n2(3), n3(1);
		n1.left = &n2;
		n1.right = &n3;
		s.recoverTree(&n1);
	}

	std::cin.get();
	return 0;
}

