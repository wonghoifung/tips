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
	void sortColors(vector<int>& nums) {
		int i = 0, j = 0;
		for (int k = 0; k < nums.size(); ++k) {
			int v = nums[k];
			nums[k] = 2;
			if (v < 2) nums[j++] = 1;
			if (v == 0) nums[i++] = 0;
		}
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




