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
	vector<int> findDisappearedNumbers(vector<int>& nums) {
		for (int i = 0; i < nums.size(); ++i) {
			if (nums[abs(nums[i]) - 1] > 0) nums[abs(nums[i]) - 1] *= -1;
		}
		vector<int> res;
		for (int i = 1; i <= nums.size(); ++i) {
			if (nums[i - 1] > 0) res.push_back(i);
		}
		return res;
	}
};

ostream& operator<<(ostream& o, const vector<int>& v) {
	o << "[ ";
	for (auto i : v) { o << i << " "; }
	o << "]" << endl;
	return o;
}

int main() {
	vector<int> nums{ 4, 3, 2, 7, 8, 2, 3, 1 };
	cout << Solution().findDisappearedNumbers(nums) << endl; // 5,6

	cin.get();
	return EXIT_SUCCESS;
}

