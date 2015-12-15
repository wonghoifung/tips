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
	int dfs(TreeNode* root, int curDep, int maxDep) {
		if (root == NULL) return maxDep;
		int cd = curDep + 1;
		if (root->left == NULL && root->right == NULL)
		{
			return maxDep < cd ? cd : maxDep;
		}
		return max(dfs(root->left, cd, maxDep), dfs(root->right, cd, maxDep));
	}
	int bfs(TreeNode* root) {
		queue<TreeNode*> q;
		q.push(root);
		int d = 0;
		int maxd = 0;
		while (q.size())
		{
			d += 1;
			int leafcount = q.size();
			for (int i = 0; i < leafcount; ++i)
			{
				TreeNode* node = q.front();
				q.pop();
				if (node->left == NULL && node->right == NULL)
				{
					if (maxd < d)
					{
						maxd = d;
					}
				}
				if (node->left) q.push(node->left);
				if (node->right) q.push(node->right);
			}
		}
		return maxd;
	}
	int maxDepth(TreeNode* root) {
		if (root == NULL) return 0;
		return bfs(root);
		/* return dfs(root, 0, 0); */
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

