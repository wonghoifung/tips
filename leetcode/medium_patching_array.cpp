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
	int minPatches(vector<int>& nums, int n) { // a sorted positive integer array nums 
		long miss = 1;
		long res = 0;
		long i = 0;
		while (miss <= n)
		{
			if (i < nums.size() && nums[i] <= miss) 
			{
				miss += nums[i++];
			}
			else 
			{
				miss += miss;
				res += 1;
			}
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1, 2, 4, 13, 43 };
		cout << s.minPatches(nums, 100) << endl;
	}

	std::cin.get();
	return 0;
}




