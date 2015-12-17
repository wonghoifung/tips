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
	TreeNode* buildTreeHelper(vector<int>& inorder, int instart, int inend, vector<int>& postorder, int poststart, int postend) {
		TreeNode* root = new TreeNode(postorder[postend]);
		int idx = findInorderIdx(inorder, instart, inend, postorder[postend]);
		int leftcount = idx - instart;
		int rightcount = inend - idx;
		if (leftcount)
		{
			root->left = buildTreeHelper(inorder, instart, idx - 1, postorder, poststart, poststart + leftcount - 1);
		}
		if (rightcount)
		{
			root->right = buildTreeHelper(inorder, idx + 1, idx + rightcount, postorder, poststart + leftcount, postend - 1);
		}
		return root;
	}
	TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
		if (inorder.empty() || inorder.size() != postorder.size())
		{
			return NULL;
		}
		return buildTreeHelper(inorder, 0, inorder.size() - 1, postorder, 0, postorder.size() - 1);
	}
};

int main()
{


	std::cin.get();
	return 0;
}

