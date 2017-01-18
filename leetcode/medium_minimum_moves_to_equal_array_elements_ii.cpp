#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
	int minMoves2(vector<int>& nums) {
		sort(nums.begin(), nums.end());
		int i = 0, j = nums.size() - 1;
		int count = 0;
		while (i < j) {
			count += nums[j] - nums[i];
			i++;
			j--;
		}
		return count;
	}
};

int main() {
	vector<int> nums{ 1, 2, 3 };
	cout << Solution().minMoves2(nums) << endl; // 2

	cin.get();
}

