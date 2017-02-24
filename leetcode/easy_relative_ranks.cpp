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
#include <functional>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
public:
	vector<string> findRelativeRanks(vector<int>& nums) {
		map<int, int, greater<int>> idxs;
		for (int i = 0; i < nums.size(); ++i) {
			idxs[nums[i]] = i;
		}
		vector<string> res(nums.size(), "");
		int i = 0;
		for (auto it = idxs.begin(); it != idxs.end(); ++it, ++i) {
			string v;
			if (i == 0) v = "Gold Medal";
			else if (i == 1) v = "Silver Medal";
			else if (i == 2) v = "Bronze Medal";
			else {
				int rank = i + 1;
				stringstream ss;
				ss << rank;
				v = ss.str();
			}
			res[it->second] = v;
		}
		return res;
	}
};

int main() {
	{
		vector<int> nums{ 5, 4, 3, 2, 1 };
		auto res = Solution().findRelativeRanks(nums);
		for (auto& s : res) cout << s << " ";
		cout << endl;
	}
	cin.get();
}

