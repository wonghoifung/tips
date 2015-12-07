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
#include <sstream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>

using namespace std;

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(NULL);
	}
	~mytimer()
	{
		time_t end = time(NULL);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	vector<int> searchRange(vector<int>& nums, int target) {
		auto low = lower_bound(nums.begin(), nums.end(), target);
		auto up = upper_bound(nums.begin(), nums.end(), target);
		if (low == nums.end() || *low != target)
		{
			return { -1, -1 };
		}
		return{ low - nums.begin(), up - nums.begin() - 1 };
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 5, 7, 7, 8, 8, 10 };
		int target = 8;
		vector<int> ret = s.searchRange(nums, target);
		copy(ret.begin(), ret.end(), ostream_iterator<int>(cout, " "));
		cout << endl;
	}
	{
		vector<int> nums{ 1 };
		int target = 0;
		vector<int> ret = s.searchRange(nums, target);
		copy(ret.begin(), ret.end(), ostream_iterator<int>(cout, " "));
		cout << endl;
	}

	std::cin.get();

	return 0;
}

