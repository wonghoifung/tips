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
#include <stdarg.h>
#include <stdio.h>

using namespace std;

class Solution {
public:
	Solution(vector<int> nums) :origine_(nums) {
		srand(time(NULL));
	}

	/** Resets the array to its original configuration and return it. */
	vector<int> reset() {
		return origine_;
	}

	/** Returns a random shuffling of the array. */
	vector<int> shuffle() {
		auto nums = origine_;
		for (int i = 1; i < nums.size(); ++i)
		{
			int j = rand() % (i + 1);
			swap(nums[i], nums[j]);
		}
		return nums;
	}
private:
	vector<int> origine_;
};

int main()
{
	auto print = [](vector<int>& v) {
		for (auto i : v) {
			cout << i << " ";
		}
		cout << endl;
	};
	vector<int> nums{ 1, 2, 3, 4, 5, 6 };
	Solution s(nums);
	auto res = s.reset();
	print(res);
	res = s.shuffle();
	print(res);
	
	std::cin.get();
	return 0;
}



