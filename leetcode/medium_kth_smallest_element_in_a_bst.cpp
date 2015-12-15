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
	TreeNode* iterativeInorder(TreeNode* root, int k) {
		stack<TreeNode*> st;
		st.push(root);
		TreeNode* n = st.top()->left;
		while (st.size())
		{
			while (n)
			{
				st.push(n);
				n = n->left;
			}

			n = st.top();
			st.pop();

			k -= 1; 
			if (k == 0) return n;

			if (n->right)
			{
				st.push(n->right);
				n = st.top()->left;
			}
			else
			{
				n = NULL;
			}
		}
		return NULL;
	}
	TreeNode* inorder(TreeNode* root, int& k) {
		if (root == NULL) return NULL;
		if (root->left)
		{
			TreeNode* l = inorder(root->left, k);
			if (l) return l;
		}
		k -= 1; 
		if (k == 0) return root;
		if (root->right)
		{
			TreeNode* r = inorder(root->right, k);
			if (r) return r;
		}
		return NULL;
	}
	int kthSmallest(TreeNode* root, int k) {
		/*TreeNode* n = inorder(root, k);*/
		TreeNode* n = iterativeInorder(root, k);
		assert(n);
		return n->val;
	}
};

int main()
{
	Solution s;
	{
		TreeNode n1(1), n2(2);
		n1.right = &n2;
		int k = 2;
		cout << s.kthSmallest(&n1, k) << endl;
	}

	std::cin.get();
	return 0;
}

