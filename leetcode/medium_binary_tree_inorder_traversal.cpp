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
	vector<int> inorderTraversalold(TreeNode* root) {
		vector<int> ret;
		stack<TreeNode*> s;
		unordered_map<TreeNode*, int> m;
		if (root) s.push(root);
		while (s.size())
		{
			TreeNode* tn = s.top();
			if (tn->left)
			{
				if (m[tn] == 0)
				{
					s.push(tn->left);
					m[tn] = 1;
				}
				else
				{
					s.pop();
					ret.push_back(tn->val);
					if (tn->right)
					{
						s.push(tn->right);
					}
				}
			}
			else 
			{
				s.pop();
				ret.push_back(tn->val);
				if (tn->right)
				{
					s.push(tn->right);
				}
			}
		}
		return ret;
	}
	vector<int> inorderTraversal(TreeNode* root) {
		vector<int> ret;
		if (root == NULL) return ret;
		stack<TreeNode*> s;
		TreeNode* p = root;
		while (p || s.size())
		{
			while (p)
			{
				s.push(p);
				p = p->left;
			}
			p = s.top(); s.pop();
			ret.push_back(p->val);
			p = p->right;
		}
		return ret;
	}
};

int main()
{
	Solution s;
	{
		TreeNode n1(1), n2(2);
		n1.left = &n2;
		vector<int> ret = s.inorderTraversal(&n1);
		copy(ret.begin(), ret.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}

