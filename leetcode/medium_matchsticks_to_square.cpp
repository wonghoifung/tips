#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
public:
	bool makesquare(vector<int>& nums) {
		int n = nums.size();
		long sum = accumulate(nums.begin(), nums.end(), 0);
		if (sum % 4) return false;
		long sideLen = sum / 4;
		vector<int> usedMasks;
		vector<bool> validHalfSubsets(1 << n, false);
		int all = (1 << n) - 1;
		for (int mask = 0; mask <= all; ++mask) {
			long subsetSum = 0;
			for (int i = 0; i < 32; ++i) {
				if ((mask >> i) & 1) subsetSum += nums[i];
			}
			if (subsetSum == sideLen) {
				for (int usedMask : usedMasks) {
					if ((usedMask & mask) == 0) {
						int validHalf = usedMask | mask;
						validHalfSubsets[validHalf] = true;
						if (validHalfSubsets[all ^ validHalf]) return true;
					}
				}
				usedMasks.push_back(mask);
			}
		}
		return false;
	}
};

int main() {
	{
		vector<int> nums{ 1, 1, 2, 2, 2 };
		cout << boolalpha << Solution().makesquare(nums) << endl;
	}
	{
		vector<int> nums{ 3, 3, 3, 3, 4 };
		cout << boolalpha << Solution().makesquare(nums) << endl;
	}

	cin.get();
}

