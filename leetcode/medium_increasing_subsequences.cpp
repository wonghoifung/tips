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
	void helper(vector<int>& nums, vector<int>& seq, set<vector<int>>& result, int index) {
		if (seq.size() >= 2) result.insert(seq);
		for (int i = index; i < nums.size(); ++i) {
			if (seq.size() == 0 || nums[i] >= seq.back()) {
				seq.push_back(nums[i]);
				helper(nums, seq, result, i + 1);
				seq.pop_back();
			}
		}
	}
public:
	vector<vector<int>> findSubsequences(vector<int>& nums) {
		set<vector<int>> res;
		vector<int> seq;
		helper(nums, seq, res, 0);
		return vector<vector<int>>(res.begin(), res.end());
	}
};

int main() {
	{
		vector<int> nums{ 4, 6, 7, 7 };
		auto res = Solution().findSubsequences(nums); // [[4, 6], [4, 7], [4, 6, 7], [4, 6, 7, 7], [6, 7], [6, 7, 7], [7,7], [4,7,7]]
		for (auto& v : res) {
			for (auto i : v) cout << i << " ";
			cout << endl;
		}
		cout << endl;
	}

	cin.get();
}

