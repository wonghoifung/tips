#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <array>
#include <memory>
#include <sstream>
#include <functional>
#include <algorithm>
#include <bitset>
#include <tuple>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

using namespace std;
#if 0
class Solution {
	multimap<int, int> mmnums;
public:
	Solution(vector<int> nums) {
		for (int i = 0; i < nums.size(); ++i) {
			mmnums.insert(make_pair(nums[i], i));
		}
	}

	int pick(int target) {
		auto p = mmnums.equal_range(target);
		if (p.first == p.second) return -1;
		vector<int> idxs;
		for (auto it = p.first; it != p.second; ++it) {
			idxs.push_back(it->second);
		}
		return idxs[rand() % idxs.size()];
	}
};
#endif
class Solution {
	vector<int> mmnums;
public:
	Solution(vector<int> nums) {
		mmnums = nums;
	}

	int pick(int target) {
		vector<int> idxs;
		for (int i = 0; i < mmnums.size(); ++i) {
			if (mmnums[i] == target) {
				idxs.push_back(i);
			}
		}
		if (idxs.empty()) return -1;
		return idxs[rand() % idxs.size()];
	}
};

int main()
{
	{
		vector<int> nums = { 1, 2, 3, 3, 3 };
		cout << Solution(nums).pick(3) << endl;
		cout << Solution(nums).pick(3) << endl;
		cout << Solution(nums).pick(3) << endl;
		cout << Solution(nums).pick(1) << endl;
		cout << Solution(nums).pick(2) << endl;
		cout << Solution(nums).pick(3) << endl;
		cout << Solution(nums).pick(3) << endl;
		cout << Solution(nums).pick(3) << endl;
	}

	std::cin.get();
	return 0;
}



