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

class BSTIterator {
private:
	TreeNode* root;
	vector<TreeNode*> q;
	int idx;
public:
	BSTIterator(TreeNode *root) {
		this->root = root;
		initq(root);
		if (q.empty())
		{
			idx = -1;
		}
		else
		{
			idx = 0;
		}
	}

	void initq(TreeNode* root) {
		if (root == NULL)
		{
			return;
		}
		initq(root->left);
		q.push_back(root);
		initq(root->right);
	}

	/** @return whether we have a next smallest number */
	bool hasNext() {
		return idx >= 0;
	}

	/** @return the next smallest number */
	int next() {
		int val = q[idx]->val;
		idx += 1;
		if (idx >= q.size())
		{
			idx = -1;
		}
		return val;
	}
};

int main()
{


	std::cin.get();

	return 0;
}

