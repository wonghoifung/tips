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

/* https://discuss.leetcode.com/topic/78993/c-with-iterators/12 */
class Solution {
	int sort_and_count(vector<int>::iterator begin, vector<int>::iterator end) {
		if (end - begin <= 1) return 0;
		auto mid = begin + (end - begin) / 2;
		int count = sort_and_count(begin, mid) + sort_and_count(mid, end);
		for (auto i = begin, j = mid; i != mid; ++i) {
			while (j != end && *i > 2L * *j) ++j;
			count += j - mid;
		}
		inplace_merge(begin, mid, end);
		return count;
	}
public:
	int reversePairs(vector<int>& nums) {
		return sort_and_count(nums.begin(), nums.end());
	}
};

int main() {
	{
		vector<int> nums{ 1, 3, 2, 3, 1 };
		cout << Solution().reversePairs(nums) << endl; // 2
	}
	{
		vector<int> nums{ 2, 4, 3, 5, 1 };
		cout << Solution().reversePairs(nums) << endl; // 3
	}

	cin.get();
}

