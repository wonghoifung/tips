#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
public:
	int totalHammingDistance(vector<int>& nums) {
		int size = nums.size();
		if (size < 2) return 0;
		int ans = 0;
		int* zeroOne = new int[2];
		while (true) {
			int zeroCount = 0;
			zeroOne[0] = 0;
			zeroOne[1] = 0;
			for (int i = 0; i < nums.size(); ++i) {
				if (nums[i] == 0) zeroCount += 1;
				zeroOne[nums[i] % 2] += 1;
				nums[i] >>= 1;
			}
			ans += zeroOne[0] * zeroOne[1];
			if (zeroCount == nums.size()) break;
		}
		return ans;
	}
};

int main() {
	{
		vector<int> nums{ 4, 14, 2 };
		cout << Solution().totalHammingDistance(nums) << endl; // 6
	}

	cin.get();
}

