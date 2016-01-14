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
	vector<string> summaryRanges(vector<int>& nums) {
		vector<string> res;
		if (nums.empty()) return res;
		stringstream ss;
		ss << nums[0]; int start = 0;
		for (int i = 1; i < nums.size(); ++i)
		{
			if (nums[i] - nums[i - 1] == 1) continue;
			if (i - 1 != start) ss << "->" << nums[i - 1];
			res.push_back(ss.str());
			ss.str("");
			ss << nums[i]; start = i;
		}
		if (ss.str().size())
		{
			if (nums.size() - 1 != start)
			{
				ss << "->" << nums.back(); 
			}
			res.push_back(ss.str());
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 0, 1, 2, 4, 5, 7 };
		vector<string> res = s.summaryRanges(nums);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n")); cout << endl;
	}
	{
		vector<int> nums{ 0, 1 };
		vector<string> res = s.summaryRanges(nums);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n")); cout << endl;
	}
	{
		vector<int> nums{ 1, 3 };
		vector<string> res = s.summaryRanges(nums);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n")); cout << endl;
	}

	std::cin.get();
	return 0;
}




