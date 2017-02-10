#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>s
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
public:
	vector<double> medianSlidingWindow(vector<int>& nums, int k) {
		multiset<int> window(nums.begin(), nums.begin() + k); // sorted
		auto mid = next(window.begin(), k / 2);
		vector<double> medians;
		for (int i = k; ; ++i) {
			medians.push_back((double(*mid) + *next(mid, k % 2 - 1)) / 2);
			if (i == nums.size()) break;

			window.insert(nums[i]);
			if (nums[i] < *mid) mid--;

			if (nums[i - k] <= *mid) mid++;
			window.erase(window.lower_bound(nums[i - k]));
		}
		return medians;
	}
};

int main() {
	{
		vector<int> nums{ 1, 3, -1, -3, 5, 3, 6, 7 };
		auto res = Solution().medianSlidingWindow(nums, 3);
		for (auto& d : res) cout << d << " ";
		cout << endl;
	}

	cin.get();
}

