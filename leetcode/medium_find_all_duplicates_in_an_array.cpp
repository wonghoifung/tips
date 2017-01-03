#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Solution {
public:
	vector<int> findDuplicates(vector<int>& nums) {
		vector<int> res;
		for (int i = 0; i < nums.size(); ++i) {
			int idx = abs(nums[i]) - 1;
			if (nums[idx] < 0) res.push_back((idx + 1));
			nums[idx] = -nums[idx];
		}
		return res;
	}
};

ostream& operator<<(ostream& o, const vector<int>& v) {
	o << "[ ";
	for (auto i : v) o << i << " ";
	o << "]" << endl;
	return o;
}
int main() {
	vector<int> nums{ 4, 3, 2, 7, 8, 2, 3, 1 };
	cout << Solution().findDuplicates(nums) << endl;

	cin.get();
	return EXIT_SUCCESS;
}

