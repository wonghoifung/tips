#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
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
	bool containsDuplicate(vector<int>& nums) {
		unordered_map<int, int> ht;
		for (size_t i = 0; i < nums.size(); ++i)
		{
			if (ht[nums[i]] != 0)
			{
				return true;
			}
			ht[nums[i]] += 1;
		}
		return false;
	}
};

int main()
{
	Solution s;

	

	std::cin.get();

	return 0;
}

