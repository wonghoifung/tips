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
	int majorityElement(vector<int>& nums) {
		int count = 0;
		int candidate = 0;
		for (size_t i = 0; i < nums.size(); ++i)
		{
			if (count == 0) {
				candidate = nums[i];
				count += 1;
			} else {
				if (candidate == nums[i]) count += 1;
				else count -= 1;
			}
		}
		return candidate;
	}
	int majorityElementSlow(vector<int>& nums) {
		sort(nums.begin(), nums.end());
		return nums[nums.size() / 2];
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

