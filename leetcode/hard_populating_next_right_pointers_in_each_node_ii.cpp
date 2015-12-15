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

struct TreeLinkNode {
	int val;
	TreeLinkNode *left, *right, *next;
	TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
};

class Solution {
public:
	void connect(TreeLinkNode* root) {
		if (root == NULL) return;
		queue<TreeLinkNode*> q;
		q.push(root);
		while (q.size())
		{
			TreeLinkNode* last = NULL;
			int s = q.size();
			for (size_t i = 0; i < s; ++i)
			{
				TreeLinkNode* node = q.front();
				q.pop();
				if (node->left) q.push(node->left);
				if (node->right) q.push(node->right);
				if (last)
				{
					last->next = node;
				}
				last = node;
			}
		}
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

