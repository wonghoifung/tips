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

class Solution {
public:
	struct bstNode
	{
		int val;
		unique_ptr<bstNode> left;
		unique_ptr<bstNode> right;
		int leftNodeCount;
		int rightNodeCount;
		int selfCount;
		bstNode(int v) :val(v) {
			leftNodeCount = 0;
			rightNodeCount = 0;
			selfCount = 1;
		}
	};

	vector<int> countSmaller(vector<int>& nums) {
		if (nums.empty())
		{
			return nums;
		}
		vector<int> counts;
		if (nums.size() == 1)
		{
			counts.push_back(0);
			return counts;
		}
		counts.resize(nums.size(), 0);
		unique_ptr<bstNode> root(new bstNode(nums.back()));
		counts[nums.size() - 1] = 0;
		for (int i = nums.size() - 2; i >= 0; --i)
		{
			int cnt = 0;
			unique_ptr<bstNode> node(new bstNode(nums[i]));
			bstNode* p = root.get(); // p is just used to traverse
			while (p)
			{
				if (p->val > nums[i])
				{
					p->leftNodeCount += 1;
					if (p->left)
					{
						p = p->left.get();
					}
					else
					{
						p->left.reset(node.release());
						break;
					}
				}
				else if (p->val < nums[i])
				{
					cnt += p->leftNodeCount;
					cnt += p->selfCount;

					p->rightNodeCount += 1;
					if (p->right)
					{
						p = p->right.get();
					}
					else
					{
						p->right.reset(node.release());
						break;
					}
				}
				else if (p->val == nums[i])
				{
					cnt += p->leftNodeCount;

					p->selfCount += 1;
					break;
				}
			}
			counts[i] = cnt;
		}
		return counts;
	}
};

int main()
{
	Solution s;
	{
		mytimer tt;
		vector<int> nums{ 5, 2, 6, 1 };
		vector<int> counts = s.countSmaller(nums);
		copy(counts.begin(), counts.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}

