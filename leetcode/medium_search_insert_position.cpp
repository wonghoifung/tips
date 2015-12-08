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
	int searchInsert(vector<int>& nums, int target) {
		if (nums.empty())
		{
			return 0;
		}
		for (size_t i = 0; i < nums.size(); ++i)
		{
			if (nums[i] >= target)
			{
				return i;
			}
		}
		return nums.size();
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

