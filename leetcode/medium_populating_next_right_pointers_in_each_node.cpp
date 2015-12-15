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
		queue<TreeLinkNode*> q1;
		q1.push(root);
		queue<TreeLinkNode*> q2;
		queue<TreeLinkNode*>* checkq = &q1;
		queue<TreeLinkNode*>* levelq = &q2;
		int qid = 1;
		while (checkq->size())
		{
			TreeLinkNode* last = NULL;
			while (checkq->size())
			{
				TreeLinkNode* node = checkq->front();
				checkq->pop();
				if (node->left) levelq->push(node->left);
				if (node->right) levelq->push(node->right);
				if (last)
				{
					last->next = node;
				}
				last = node;
			}
			queue<TreeLinkNode*>* tmp = checkq;
			checkq = levelq;
			levelq = tmp;
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

