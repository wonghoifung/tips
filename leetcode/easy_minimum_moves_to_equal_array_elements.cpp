#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

/* https://discuss.leetcode.com/topic/66737/it-is-a-math-question */
class Solution {
public:
	int minMoves(vector<int>& nums) {
		if (nums.size() < 2) return 0;
		int minnum = nums[0];
		for (int n : nums) minnum = min(minnum, n);
		int res = 0;
		for (int n : nums) res += n - minnum;
		return res;
	}
};

int main() {
	vector<int> nums{ 1, 2, 3 };
	cout << Solution().minMoves(nums) << endl; // 3

	cin.get();
	return EXIT_SUCCESS;
}

