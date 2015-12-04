#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
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
	vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
		vector<vector<int>> result;
		if (root == NULL)
		{
			return result;
		}
		bool left2right = true;
		deque<TreeNode*> q;
		q.push_back(root);
		while (q.size())
		{
			if (left2right)
			{
				left2right = false;
				queue<TreeNode*> levelnode;
				while (q.size())
				{
					levelnode.push(q.front());
					q.pop_front();
				}
				vector<int> levelresult;
				while (levelnode.size())
				{
					TreeNode* n = levelnode.front();
					levelnode.pop();
					levelresult.push_back(n->val);
					if (n->left)
					{
						q.push_back(n->left);
					}
					if (n->right)
					{
						q.push_back(n->right);
					}
				}
				if (levelresult.size())
				{
					result.push_back(levelresult);
				}
			}
			else
			{
				left2right = true;
				stack<TreeNode*> levelnode;
				while (q.size())
				{
					levelnode.push(q.front());
					q.pop_front();
				}
				vector<int> levelresult;
				while (levelnode.size())
				{
					TreeNode* n = levelnode.top();
					levelnode.pop();
					levelresult.push_back(n->val);
					if (n->right)
					{
						q.push_front(n->right);
					}
					if (n->left)
					{
						q.push_front(n->left);
					}
				}
				if (levelresult.size())
				{
					result.push_back(levelresult);
				}
			}
		}
		return result;
	}
};

int main()
{
	TreeNode root(1);
	vector<vector<int>> result = Solution().zigzagLevelOrder(&root);

	std::cin.get();

	return 0;
}

