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
#include <array>
#include <memory>
#include <sstream>
#include <functional>
#include <algorithm>
#include <bitset>
#include <tuple>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <stdarg.h>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
	int _rob(TreeNode* root, int& l, int& r) {
		if (!root) return 0;
		int ll = 0, lr = 0, rl = 0, rr = 0;
		l = _rob(root->left, ll, lr);
		r = _rob(root->right, rl, rr);
		return max(root->val + ll + lr + rl + rr, l + r);
	}
	int rob(TreeNode* root) {
		int l, r;
		return _rob(root, l, r);
	}
};

int main()
{
	Solution s;

	std::cin.get();
	return 0;
}




