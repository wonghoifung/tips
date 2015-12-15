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
	void sumNumbersHelper(TreeNode* root, int num, int& sum) {
		if (root == NULL)
		{
			return;
		}
		num *= 10;
		num += root->val;
		if (root->left == NULL && root->right == NULL)
		{
			sum += num;
			return;
		}
		sumNumbersHelper(root->left, num, sum);
		sumNumbersHelper(root->right, num, sum);
	}
	int sumNumbers(TreeNode* root) {
		int num = 0;
		int sum = 0;
		sumNumbersHelper(root, num, sum);
		return sum;
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

