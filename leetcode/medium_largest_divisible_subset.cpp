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

using namespace std;

class Solution {
public:
	vector<int> largestDivisibleSubset(vector<int>& nums) {
		sort(nums.begin(), nums.end());

		vector<int> dp(nums.size(), 0); // max length
		vector<int> parent(nums.size(), 0); // next pointer

		int m = 0;
		int mi = 0;

		for (int i = nums.size() - 1; i >= 0; --i) 
		{
			for (int j = i; j < nums.size(); ++j)
			{
				if (nums[j] % nums[i] == 0 && dp[i] < 1 + dp[j])
				{
					dp[i] = 1 + dp[j];
					parent[i] = j;

					if (dp[i] > m)
					{
						m = dp[i];
						mi = i;
					}
				}
			}
		}

		vector<int> ret;

		for (int i = 0; i < m; ++i)
		{
			ret.push_back(nums[mi]);
			mi = parent[mi];
		}

		return ret;
	}
};

int main()
{
	auto printVector = [](const vector<int>& v) {
		for (auto i : v) {
			cout << i << " ";
		}
		cout << endl;
	};

	{
		vector<int> v = { 1, 2, 3 };
		printVector(Solution().largestDivisibleSubset(v)); // 1,2
	}

	{
		vector<int> v = { 1, 2, 4, 8 };
		printVector(Solution().largestDivisibleSubset(v)); // 1,2,4,8
	}

	std::cin.get();
	return 0;
}




