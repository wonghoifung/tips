#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
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

class Solution {
public:
	int help_search(vector<int>& nums, int start, int end, int target)
	{
		if (nums.empty() || start > end || start < 0 || end >= nums.size())
		{
			return -1;
		}
		int idx = (start + end) / 2;
		if (nums[idx] == target)
		{
			return idx;
		}
		int l = help_search(nums, start, idx - 1, target);
		if (l != -1)
		{
			return l;
		}
		int r = help_search(nums, idx + 1, end, target);
		if (r != -1)
		{
			return r;
		}
	}
	int search(vector<int>& nums, int target) {
		return help_search(nums, 0, nums.size() - 1, target);
	}
};

int main()
{
	Solution s;

	{
		mytimer t;
		vector<int> nums{ 7, 8, 9, 1, 2, 3, 4, 5, 6 };
		int target = 9;
		cout << s.search(nums, target) << endl;
	}

	std::cin.get();

	return 0;
}

