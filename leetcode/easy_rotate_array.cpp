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
	void rotate(vector<int>& nums, int k) {
		int len = nums.size();
		k %= nums.size();
		if (nums.empty() || k == 0)
		{
			return;
		}
		int h = nums.size() - k; 
		for (size_t i = 0; i < h; i++)
		{
			nums.push_back(nums[i]);
		}
		int n = h;
		while (n--)
		{
			nums.erase(nums.begin());
		}
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1, 2, 3 };
		cout << "1: "; copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " ")); cout << endl;
		int k = 1;
		s.rotate(nums, k);
		cout << "4: "; copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " "));
	}
	std::cin.get();

	return 0;
}

