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
	bool increasingTriplet(vector<int>& nums) {
		/* O(n) time complexity and O(1) space complexity */
		int minv = INT_MAX;
		int secondminv = INT_MAX;
		for (int i : nums) {
			if (i <= minv) minv = i;
			else if (i < secondminv) secondminv = i;
			else if (i > secondminv) {
				/*cout << minv << ", " << secondminv << ", " << i << endl;*/
				return true;
			}
		}
		return false;
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 1, 2, 3, 4, 5 };
		cout << boolalpha << s.increasingTriplet(nums) << endl;
	}
	{
		vector<int> nums{ 5, 4, 3, 2, 1 };
		cout << boolalpha << s.increasingTriplet(nums) << endl;
	}
	{
		vector<int> nums{ 5, 2, 4, 1, 5, 2, 2 };
		cout << boolalpha << s.increasingTriplet(nums) << endl;
	}

	std::cin.get();
	return 0;
}




