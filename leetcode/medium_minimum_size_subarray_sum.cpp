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
#include <bitset>
#include <numeric>
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
	int minSubArrayLen(int s, vector<int>& nums) {
		int minCnt = INT_MAX;
		bool found = false;
		for (size_t start = 0; start < nums.size(); ++start)
		{
			int sum = 0;
			int cnt = 0;
			for (size_t i = start; i < nums.size(); ++i)
			{
				cnt += 1;
				sum += nums[i];
				if (sum < s)
				{
					continue;
				}
				if (sum >= s)
				{
					found = true;
					minCnt = min(cnt, minCnt);
					break;
				}
			}
		}
		return found ? minCnt : 0;
	}
};

int main()
{
	Solution sl;
	{
		int s = 11;
		vector<int> nums{ 1, 2, 3, 4, 5 };
		cout << sl.minSubArrayLen(s, nums) << endl;
	}
	
	std::cin.get();

	return 0;
}

