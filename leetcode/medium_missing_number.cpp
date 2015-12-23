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
	int missingNumber(vector<int>& nums) {
		int d = 0;
		for (size_t i = 1; i <= nums.size(); ++i)
		{
			d ^= i ^ nums[i - 1];
		}
		return d;
	}
	int missingNumberFaster(vector<int>& nums) {
		int d = 0; 
		for (size_t i = 0; i < nums.size(); ++i)
		{
			d += i;
			d -= nums[i];
		}
		return d + nums.size();
	}
	int missingNumberSlow(vector<int>& nums) {
		sort(nums.begin(), nums.end());
		for (size_t i = 0; i < nums.size() - 1; ++i)
		{
			if (nums[i + 1] != nums[i] + 1)
			{
				return nums[i] + 1;
			}
		}
		return nums[0] ? 0 : nums.back() + 1;
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

