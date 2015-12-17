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
	int height(TreeNode* root) {
		if (root == NULL) return 0;
		return max(height(root->left), height(root->right)) + 1;
	}

	vector<vector<int>> levelOrderBottom(TreeNode* root) {
		int h = height(root);
		if (h == 0) return vector<vector<int>>();
		vector<vector<int>> ret(h, vector<int>());
		queue<TreeNode*> q;
		if (root) q.push(root);
		int level = h - 1;
		while (q.size())
		{
			int s = q.size();
			vector<int>& line = ret[level];
			for (size_t i = 0; i < s; ++i)
			{
				TreeNode* tn = q.front();
				q.pop();
				line.push_back(tn->val);
				if (tn->left) q.push(tn->left);
				if (tn->right) q.push(tn->right);
			}
			level -= 1;
		}
		return ret;
	}
};
#if 0
class Solution {
public:
	vector<vector<int>> levelOrderBottom(TreeNode* root) {
		vector<vector<int>> ret;
		queue<TreeNode*> q;
		if (root) q.push(root);
		while (q.size())
		{
			int s = q.size();
			vector<int> line;
			for (size_t i = 0; i < s; ++i)
			{
				TreeNode* tn = q.front();
				q.pop();
				line.push_back(tn->val);
				if (tn->left) q.push(tn->left);
				if (tn->right) q.push(tn->right);
			}
			ret.push_back(line);
		}
		reverse(ret.begin(), ret.end());
		return ret;
	}
};
#endif
int main()
{
	Solution s;
	{

	}

	std::cin.get();
	return 0;
}

