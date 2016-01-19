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

using namespace std;

class Solution {
public:
	int findPeakElement(vector<int>& nums) {
		if (nums.size() < 2) return 0;
		for (int i = 0; i < nums.size(); ++i)
		{
			if (i == 0 && i + 1 < nums.size() && nums[i] > nums[i + 1]) return i;
			if (i > 0 && i + 1 < nums.size() && nums[i] > nums[i - 1] && nums[i] > nums[i + 1]) return i;
			if (i == nums.size() - 1 && i > 0 && nums[i] > nums[i - 1]) return i;
		}
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 4, 3, 1, 4, 2 };
	}

	std::cin.get();
	return 0;
}




