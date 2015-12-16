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
	int countNodes(TreeNode* root) {
		if (!root) return 0;
		TreeNode *left = root, *right = root;
		int d = 0;
		while (left && right)
		{
			d += 1;
			left = left->left;
			right = right->right;
		}
		if (!left && !right) return (1 << d) - 1;
		return countNodes(root->left) + 1 + countNodes(root->right);
	}
};

int main()
{
	Solution s;
	{
		TreeNode n1(1), n2(2), n3(3), n4(4);
		n1.left = &n2;
		n1.right = &n3;
		n2.left = &n4;
		cout << s.countNodes(&n1) << endl;
	}

	std::cin.get();
	return 0;
}

