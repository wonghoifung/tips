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
	int findMaxConsecutiveOnes(vector<int>& nums) {
		int res = 0;
		int cur = 0;
		bool start = true;
		for (auto n : nums) {
			if (n == 1) {
				start = false;
				cur += 1;
			}
			else {
				if (!start) {
					res = max(cur, res);
					cur = 0;
					start = true;
				}
			}
		}
		return max(cur, res);
	}
};

int main() {
	{
		vector<int> nums{ 1, 1, 0, 1, 1, 1 };
		cout << Solution().findMaxConsecutiveOnes(nums) << endl; // 3
	}

	cin.get();
}

