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
	int minDepthHelper(TreeNode* root, int currentMinD, int lastMinD) {
		int d = currentMinD + 1;
		if (root->left == NULL && root->right == NULL)
		{
			return d < lastMinD ? d : lastMinD;
		}
		if (root->left)
		{
			int ld = minDepthHelper(root->left, d, lastMinD);
			if (ld < lastMinD)
			{
				lastMinD = ld;
			}
		}
		if (root->right)
		{
			int rd = minDepthHelper(root->right, d, lastMinD);
			if (rd < lastMinD)
			{
				lastMinD = rd;
			}
		}
		return lastMinD;
	}
	int minDepth(TreeNode* root) {
		if (root == NULL) return 0;
		return minDepthHelper(root, 0, INT_MAX);
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

