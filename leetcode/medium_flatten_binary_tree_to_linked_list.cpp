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
	void preorder(TreeNode* root, queue<TreeNode*>& q) {
		if (root == NULL) return;
		TreeNode* rootleft = root->left;
		TreeNode* rootright = root->right;
		root->left = NULL;
		root->right = NULL;
		if (q.size())
		{
			q.back()->right = root;
		}
		q.push(root);
		if (rootleft) preorder(rootleft, q);
		if (rootright) preorder(rootright, q);
	}
	void flatten(TreeNode* root) {
		if (root == NULL) return;
		queue<TreeNode*> q;
		preorder(root, q);
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

