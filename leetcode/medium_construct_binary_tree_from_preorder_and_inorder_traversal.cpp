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
	int findInorderIdx(vector<int>& inorder, int instart, int inend, int val) {
		for (int idx = instart; idx <= inend; ++idx)
		{
			if (inorder[idx] == val)
			{
				return idx;
			}
		}
		assert(0);
		return -1;
	}
	TreeNode* buildTreeHelper(vector<int>& preorder, int prestart, int preend, vector<int>& inorder, int instart, int inend) {
		TreeNode* root = new TreeNode(preorder[prestart]);
		int idx = findInorderIdx(inorder, instart, inend, preorder[prestart]);
		int leftcount = idx - instart;
		int rightcount = inend - idx;
		if (leftcount)
		{
			root->left = buildTreeHelper(preorder, prestart + 1, prestart + leftcount, inorder, instart, idx - 1);
		}
		if (rightcount)
		{
			root->right = buildTreeHelper(preorder, prestart + leftcount + 1, preend, inorder, idx + 1, inend);
		}
		return root;
	}
	TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
		if (preorder.empty() || preorder.size() != inorder.size())
		{
			return NULL;
		}
		return buildTreeHelper(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
	}
};

int main()
{


	std::cin.get();
	return 0;
}

