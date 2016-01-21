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
	int threeSumClosest(vector<int>& nums, int target) {
		int diff = INT_MAX;
		sort(nums.begin(), nums.end());
		int len = nums.size();
		for (int i = 0; i < len - 2; ++i) {
			int p = i + 1;
			int q = len - 1;
			while (p < q)
			{
				int d = nums[i] + nums[p] + nums[q] - target;
				if (d == 0) return target;
				diff = abs(d) < abs(diff) ? d : diff;
				if (d > 0) --q;
				else ++p;
			}
		}
		return target + diff;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1, 2, 4, 8, 16, 32, 64, 128 };
		auto res = s.threeSumClosest(nums, 82);
		cout << res << endl;
	}

	std::cin.get();
	return 0;
}




