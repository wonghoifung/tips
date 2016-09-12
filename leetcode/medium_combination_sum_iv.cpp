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
#include <stdio.h>

using namespace std;

class Solution {
public:
	int combinationSum4(vector<int>& nums, int target) {
		sort(nums.begin(), nums.end());
		int count = 0;
		vector<int> cnts(target + 1, 0);
		for (int t = 1; t <= target; ++t) {
			for (int j = 0; j < nums.size(); ++j) {
				if (nums[j] > t) break;
				else if (nums[j] == t) cnts[t] += 1;
				else cnts[t] += cnts[t - nums[j]];
			}
		}
		return cnts[target];
	}
};

int main()
{
	{
		vector<int> nums{ 1, 2, 3 };
		int target = 4;
		cout << Solution().combinationSum4(nums, target) << endl; 
	}

	std::cin.get();
	return 0;
}




