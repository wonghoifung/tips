#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <deque>
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
	vector<int> rightSideView(TreeNode* root) {
		vector<int> result;
		if (root)
		{
			deque<TreeNode*> q;
			q.push_back(root);
			while (q.size())
			{
				int count = q.size();
				result.push_back(q[count - 1]->val);
				for (int i = 0; i < count; ++i)
				{
					TreeNode* t = q.front();
					q.pop_front();
					if (t->left)
					{
						q.push_back(t->left);
					}
					if (t->right)
					{
						q.push_back(t->right);
					}
				}
			}
		}
		return result;
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

