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
		start_ = time(nullptr);
	}
	~mytimer()
	{
		time_t end = time(nullptr);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	int singleNumber(vector<int>& nums) {
		int counts[32] = { 0 };
		for (const auto& i : nums) {
			for (int j = 0; j < 32; ++j)
			{
				if (i & (1 << j))
				{
					counts[j] += 1;
				}
			}
		}
		int res = 0;
		for (int k = 31; k >= 0; --k)
		{
			if (counts[k] % 3)
			{
				res += (1 << k);
			}
		}
		return res;
	}
	int singleNumber1(vector<int>& nums) {
		sort(nums.begin(), nums.end());
		for (size_t i = 0; i < nums.size(); i += 3)
		{
			if ((i >= nums.size() - 2) || (nums[i] != nums[i + 1] && nums[i] != nums[i + 2]))
			{
				return nums[i];
			}
		}
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1 };
		cout << s.singleNumber(nums) << endl;
	}

	std::cin.get();
	return 0;
}

