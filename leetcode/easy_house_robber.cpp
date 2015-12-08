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
	typedef pair<int, int>  rang;
	map<rang, int> rangsum;
	int maxsum(vector<int>& nums, int start, int end) {
		if (start > end)
		{
			return 0;
		}
		if (start == end)
		{
			nums[start];
		}
		auto it = rangsum.find({ start, end });
		if (it != rangsum.end())
		{
			return it->second;
		}
		int s1 = nums[start] + maxsum(nums, start + 2, end);
		int s2 = maxsum(nums, start + 1, end);
		int ret = max(s1, s2);
		rangsum[{start, end}] = ret;
		return ret;
	}
	int rob(vector<int>& nums) {
		if (nums.empty())
		{
			return 0;
		}
		return maxsum(nums, 0, nums.size() - 1);
	}
};

int main()
{
	Solution s;
	{
		mytimer t;
		vector<int> nums{ 183, 219, 57, 193, 94, 233, 202, 154, 65, 240, 97, 234, 100, 249, 186, 66, 90, 238, 168, 128, 177, 235, 50, 81, 185, 165, 217, 207, 88, 80, 112, 78, 135, 62, 228, 247, 211 };
		cout << s.rob(nums) << endl;
	}
	std::cin.get();

	return 0;
}

