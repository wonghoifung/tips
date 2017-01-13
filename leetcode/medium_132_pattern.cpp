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

class Solution {
public:
	bool find132pattern(vector<int>& nums) {
		if (nums.size() <= 2) return false;
		int n = nums.size();
		int i = 0, j = 0, k = 0;
		while (i < n) {
			while (i + 1 < n && nums[i] >= nums[i + 1]) i++;
			j = i + 1;
			while (j + 1< n && nums[j] <= nums[j + 1]) j++;
			k = j + 1;
			while (k < n) {
				if (nums[k] < nums[j] && nums[k] > nums[i]) return true;
				k++;
			}
			i = j + 1;
		}
		return false;
	}
};

int main() {
	{
		vector<int> nums{ 1, 2, 3, 4 };
		cout << boolalpha << Solution().find132pattern(nums) << endl; // false
	}
	{
		vector<int> nums{ 3, 1, 4, 2 };
		cout << boolalpha << Solution().find132pattern(nums) << endl; // true
	}
	{
		vector<int> nums{ -1, 3, 2, 0 };
		cout << boolalpha << Solution().find132pattern(nums) << endl; // true
	}
	{
		vector<int> nums{ 8, 10, 4, 6, 5 };
		cout << boolalpha << Solution().find132pattern(nums) << endl; // true
	}

	cin.get();
	return EXIT_SUCCESS;
}

