nclude <string>
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
	vector<int> postorderTraversal(TreeNode* root) {
		vector<int> ret;
		stack<TreeNode*> s;
		unordered_map<TreeNode*, int> m;
		if (root) s.push(root);
		while (s.size())
		{
			TreeNode* p = s.top();
			if (p->left == NULL && p->right == NULL)
			{
				ret.push_back(p->val);
				s.pop();
				continue;
			}
			if (m[p] == 0)
			{
				if (p->right) s.push(p->right);
				if (p->left) s.push(p->left);
				m[p] = 1;
			}
			else
			{
				ret.push_back(p->val);
				s.pop();
			}
		}
		return ret;
	}
	vector<int> postorderTraversal2(TreeNode* root) {
		vector<int> ret;
		stack<TreeNode*> s;
		if (root) s.push(root);
		while (s.size())
		{
			TreeNode* curr = s.top();
			s.pop();
			if (curr->left) s.push(curr->left);
			if (curr->right) s.push(curr->right);
			ret.push_back(curr->val);
		}
		reverse(ret.begin(), ret.end());
		return ret;
	}
};

int main()
{
	Solution s;
	{
		TreeNode n1(1), n2(2);
		n1.left = &n2;
		vector<int> ret = s.postorderTraversal(&n1);
		copy(ret.begin(), ret.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}

