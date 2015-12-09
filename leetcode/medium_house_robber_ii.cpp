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
	int maxsum(vector<int>& nums, int start, int end) {
		if (start >= end)
		{
			return 0;
		}
		int lastmax = 0, currentmax = nums[start];
		for (int i = start + 1; i < end; ++i)
		{
			int temp = currentmax;
			currentmax = max(lastmax + nums[i], temp);
			lastmax = temp;
		}
		return currentmax;
	}
	int rob(vector<int>& nums) {
		int n = nums.size();
		if (n == 1)
		{
			return nums[0];
		}
		return max(maxsum(nums, 0, n - 1), maxsum(nums, 1, n));
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1, 2, 1, 1 };
		cout << s.rob(nums) << endl;
	}
	std::cin.get();

	return 0;
}

