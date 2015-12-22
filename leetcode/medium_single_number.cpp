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
		int res = nums[0];
		for (size_t i = 1; i < nums.size(); ++i)
		{
			res ^= nums[i];
		}
		return res;
	}
	int singleNumber1(vector<int>& nums) {
		sort(nums.begin(), nums.end());
		for (size_t i = 0; i < nums.size(); i += 2)
		{
			if (i == nums.size() - 1 || nums[i] != nums[i + 1])
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
		
	}

	std::cin.get();
	return 0;
}

