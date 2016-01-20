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
	bool containsNearbyDuplicate(vector<int>& nums, int k) {
		unordered_map<int, int> m;
		for (int i = 0; i < nums.size(); ++i)
		{
			auto it = m.find(nums[i]);
			if (it != m.end() && i - it->second <= k) return true;
			m[nums[i]] = i;
		}
		return false;
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




