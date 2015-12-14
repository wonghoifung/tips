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
	bool equal(TreeNode* l, TreeNode* r) {
		if (l == NULL && r == NULL)
		{
			return true;
		}
		if (!(l && r))
		{
			return false;
		}
		if (l->val != r->val)
		{
			return false;
		}
		return equal(l->left, r->left) && equal(l->right, r->right);
	}
	void insert(TreeNode* root, int n) {
		assert(root);
		if (root->val > n)
		{
			if (root->left == NULL)
			{
				root->left = new TreeNode(n);
			}
			else
			{
				insert(root->left, n);
			}
		}
		else
		{
			if (root->right == NULL)
			{
				root->right = new TreeNode(n);
			}
			else
			{
				insert(root->right, n);
			}
		}
	}
	TreeNode* build(vector<int>& seq) {
		if (seq.empty()) return NULL;
		TreeNode* root = new TreeNode(seq[0]);
		for (int i = 1; i < seq.size(); ++i)
		{
			insert(root, seq[i]);
		}
		return root;
	}
	bool found(TreeNode* root, vector<TreeNode*>& exists) {
		bool ret = false;
		for (size_t i = 0; i < exists.size(); ++i)
		{
			if (equal(root, exists[i]))
			{
				return true;
			}
		}
		return ret;
	}
	vector<TreeNode*> generateTrees(int n) {
		vector<TreeNode*> ret;
		if (n == 0) return ret;
		vector<int> seq;
		for (size_t i = 1; i <= n; ++i)
		{
			seq.push_back(i);
		}
		do
		{
			TreeNode* nseq = build(seq);
			if (!found(nseq, ret))
			{
				ret.push_back(nseq);
			}
		} while (next_permutation(seq.begin(), seq.end()));
		return ret;
	}
};
#endif

class Solution {
public:
	vector<TreeNode*> get(int a, int b) {
		vector<TreeNode*> ret;
		if (a>b) {
			ret.push_back(NULL);
		}
		else {
			for (int i = a; i <= b; i++)
				for (auto l : get(a, i - 1))
					for (auto r : get(i + 1, b))
					{
						ret.push_back(new TreeNode(i));
						ret.back()->left = l;
						ret.back()->right = r;
					}
		}
		return ret;
	}

	vector<TreeNode*> generateTrees(int n) {
		if (n == 0) return vector<TreeNode*>();
		return get(1, n);
	}
};

int main()
{
	Solution s;
	{
		int n = 3;
		vector<TreeNode*> alltrees = s.generateTrees(n);
		cout << alltrees.size() << endl;
	}

	std::cin.get();
	return 0;
}

