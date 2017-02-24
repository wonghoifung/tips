#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
public:
	vector<int> nextGreaterElements(vector<int>& nums) {
		vector<int> res;
		for (int i = 0; i < nums.size(); ++i) {
			bool found = false;
			for (int j = (i + 1) % nums.size(), tmp = 0;
				tmp < nums.size() - 1;
				j = (j + 1) % nums.size(), ++tmp) {
				if (nums[j] > nums[i]) {
					res.push_back(nums[j]);
					found = true;
					break;
				}
			}
			if (!found) res.push_back(-1);
		}
		return res;
	}
};

int main() {
	{
		vector<int> nums{ 1, 2, 1 };
		auto res = Solution().nextGreaterElements(nums);
		for (auto i : res) cout << i << " ";
		cout << endl;
	}
	cin.get();
}

